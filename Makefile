# Makefile for simpleOS

# for details: https://github.com/cfenollosa/os-tutorial/
# James Molloy's tutorial: https://web.archive.org/web/20160326060959/http://jamesmolloy.co.uk/tutorial_html/2.-Genesis.html
# Bran's tutorial: https://web.archive.org/web/20160330040433/http://www.osdever.net/bkerndev/index.php
# Little OS book: https://littleosbook.github.io/#booting


NASM ?= nasm
CC   ?= cc
LD   ?= ld
GDB = /usr/local/i386elfgcc/bin/i386-elf-gdb

OPTS += -O2 -g
WARN += -Wall -Wextra -Werror
INCL += -I.
CFLAGS += -fno-stack-protector -fno-builtin -nostdinc -nostdlib \
          -nostartfiles -nodefaultlibs -m32 -std=c99 $(OPTS)
LDFLAGS += -melf_i386

CAT ?= cat
MV  ?= mv
ECHO ?= echo
MKDIR ?= mkdir
PRINTF ?= printf
RM ?= rm -f
CP ?= cp
GENISOIMAGE ?= genisoimage
QEMU ?= qemu-system-i386

MAKE = make

VER=0.0.1


BUILD_DIR := build
OBJ_DIR := $(BUILD_DIR)/obj
KERNEL_ELF := $(BUILD_DIR)/kernel.elf
IMGFILE=$(BUILD_DIR)/simpleOS-$(VER).iso

SRC_DIRS := $(shell ls -d */)

C_SRC_FILES := $(foreach dir,$(SRC_DIRS),$(addprefix $(OBJ_DIR)/,$(wildcard $(dir)*.c)))
ASM_SRC_FILES := $(foreach dir,$(SRC_DIRS),$(addprefix $(OBJ_DIR)/,$(wildcard $(dir)*.asm)))

C_OBJ_FILES := $(C_SRC_FILES:.c=.o)
C_DEP_FILES := $(C_SRC_FILES:.c=.d)
ASM_OBJ_FILES := $(ASM_SRC_FILES:.asm=.o)

OBJS := $(C_OBJ_FILES) $(ASM_OBJ_FILES)

.PHONY: emulate
.SECONDARY: $(KERNEL_ELF) $(BUILD_DIR)/iso/boot/grub/menu.lst

all : $(KERNEL_ELF)

-include $(C_DEP_FILES)

$(OBJ_DIR)/%.o: %.asm
	@$(MKDIR) -p $(shell dirname $@)
	@$(PRINTF) 'Assembling \033[1m$<\033[0m...\n'
	$(NASM) -f elf32 -o $@ $^

$(OBJ_DIR)/%.o: %.c
	@$(MKDIR) -p $(shell dirname $@)
	@$(CC) $(CFLAGS) $(INCL) $(WARN) -c -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(PRINTF) 'Compiling  \033[1m$<\033[0m...\n'
	$(CC) $(CFLAGS) $(INCL) $(WARN) -c $< -o $@

$(KERNEL_ELF): ${OBJS}
	@$(PRINTF) 'Linking    \033[1m$@\033[0m...\n'
	$(LD) $(LDFLAGS) -T link.ld $(OBJ_DIR)/boot/loader.o $(filter-out $(OBJ_DIR)/boot/loader.o,$(OBJS)) -o $@

$(BUILD_DIR)/iso/boot/grub/menu.lst: $(KERNEL_ELF)
	@$(PRINTF) 'Preparing  \033[1miso/grub/\033[0m...\n'
	@$(MKDIR) -p $(shell dirname $@)
	$(CP) boot/stage2_eltorito $(shell dirname $@)
	$(CP) $^ $(BUILD_DIR)/iso/boot
	$(PRINTF) "default=0\ntimeout=0\n\ntitle simpleOS\nkernel /boot/$(shell basename $^)" > $(shell dirname $@)/menu.lst

# $(IMGFILE): iso/boot/grub/menu.lst
# 	@$(PRINTF) 'Generating \033[1m$(IMGFILE)\033[0m...\n'
# 	$(GENISOIMAGE) \
# 		-R -b $(BUILD_DIR)/boot/grub/stage2_eltorito \
# 		-no-emul-boot -boot-load-size 4 \
# 		-A simpleOS -input-charset utf8 \
# 		-quiet -boot-info-table -o $@ iso

$(IMGFILE): $(BUILD_DIR)/iso/boot/grub/menu.lst
	@$(PRINTF) 'Generating \033[1m$(IMGFILE)\033[0m...\n'
	cd $(BUILD_DIR) && $(GENISOIMAGE) \
		-R -b boot/grub/stage2_eltorito \
		-no-emul-boot -boot-load-size 4 \
		-quiet -boot-info-table -o $(shell basename $@) iso

image: $(IMGFILE)

clean:
	$(RM) -r $(BUILD_DIR)

emulate: $(IMGFILE)
	@$(PRINTF) 'Emulating  \033[1m$<\033[0m...\n'
	$(PRINTF) "" > $(BUILD_DIR)/serial.log
	$(QEMU) -cdrom $(IMGFILE) -serial file:$(BUILD_DIR)/serial.log &
	tail -f $(BUILD_DIR)/serial.log

debug: $(IMGFILE)
	$(QEMU) -cdrom $(IMGFILE) -s -S &
	$(GDB) --symbol=kernel.elf -ex 'target remote localhost:1234'
