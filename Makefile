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
INCL += -I$(shell pwd)
CFLAGS += -fno-stack-protector -fno-builtin -nostdinc -nostdlib \
          -nostartfiles -nodefaultlibs -m32 -std=c99 $(OPTS)
LDFLAGS += -melf_i386

CAT ?= cat
MV  ?= mv
ECHO ?= echo
MKDIR ?= mkdir

#PRINTF ?= $(SHELL) -c ""
PRINTF ?= printf

RM ?= rm -f

CP ?= cp
GENISOIMAGE ?= genisoimage
QEMU ?= qemu-system-i386

MAKE = make

VER=0.0.1

IMGFILE=simpleOS-$(VER).img

SRC_DIRS := cpu util boot drivers kernel

C_SRC_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
ASM_SRC_FILES := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.asm))

C_OBJ_FILES := $(C_SRC_FILES:.c=.o)
C_DEP_FILES := $(C_SRC_FILES:.c=.d)
ASM_OBJ_FILES := $(ASM_SRC_FILES:.asm=.o)

OBJS := $(C_OBJ_FILES) $(ASM_OBJ_FILES)

.PHONY: emulate
.SECONDARY: kernel.elf iso/boot/grub/menu.lst

all : kernel.elf

-include $(C_DEP_FILES)

%.o: %.asm
	@$(PRINTF) ' -- Assembling \033[1m$<\033[0m...\n'
	$(NASM) -f elf32 -o $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) $(INCL) $(WARN) -c -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(PRINTF) ' -- Compiling  \033[1m$<\033[0m...\n'
	$(CC) $(CFLAGS) $(INCL) $(WARN) -c $< -o $@

kernel.elf: ${OBJS}
	@$(PRINTF) ' -- Linking    \033[1m$@\033[0m...\n'
	$(LD) $(LDFLAGS) -T link.ld $(OBJS) -o $@

iso/boot/grub/menu.lst: kernel.elf
	@$(PRINTF) ' -- Preparing  \033[1miso/grub/\033[0m...\n'
	@$(MKDIR) -p iso/boot/grub
	$(CP) stage2_eltorito iso/boot/grub
	$(CP) $^ iso/boot
	printf "default=0\ntimeout=0\n\ntitle simpleOS\nkernel /boot/$^" > iso/boot/grub/menu.lst

$(IMGFILE): iso/boot/grub/menu.lst
	@$(PRINTF) ' -- Generating \033[1m$(IMGFILE)\033[0m...\n'
	$(GENISOIMAGE) \
		-R -b boot/grub/stage2_eltorito \
		-no-emul-boot -boot-load-size 4 \
		-A simpleOS -input-charset utf8 \
		-quiet -boot-info-table -o $@ iso

clean:
	$(RM) $(OBJS)
	$(RM) $(C_DEP_FILES)
	$(RM) -r iso/
	$(RM) *.elf
	$(RM) $(IMGFILE)
	$(RM) *.log

emulate: $(IMGFILE)
	@$(PRINTF) ' -- Emulating  \033[1m$<\033[0m...\n'
	touch serial.log
	$(QEMU) -cdrom $(IMGFILE) -serial file:serial.log &
	tail -f serial.log

debug: $(IMGFILE)
	$(QEMU) -cdrom $(IMGFILE) -s -S &
	$(GDB) --symbol=kernel.elf -ex 'target remote localhost:1234'
