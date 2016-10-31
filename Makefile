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
PRINTF ?= $(SHELL) -c ""
CP ?= cp
GENISOIMAGE ?= genisoimage
QEMU ?= qemu-system-i386

MAKE = make

VER=0.0.1

IMGFILE=simpleOS-$(VER).img

export

.PHONY: emulate build-boot build-util build-drivers build-kernel
.SECONDARY: kernel.elf iso/boot/grub/menu.lst

all : kernel.elf

build-boot:
	@$(MAKE) -C boot all

build-cpu:
	@$(MAKE) -C cpu all

build-util:
	@$(MAKE) -C util all

build-drivers:
	@$(MAKE) -C drivers all

build-kernel:
	@$(MAKE) -C kernel all

kernel.elf: build-boot build-cpu build-util build-drivers build-kernel link.ld
	$(eval objfiles_all=$$(shell find * | grep '\.o'))
	$(eval objfiles=$$(filter-out boot/loader.o,$$(objfiles_all)))
	@$(PRINTF) ' -- Linking    \033[1m$@\033[0m...\n'
	$(LD) $(LDFLAGS) -T link.ld boot/loader.o $(objfiles) -o $@

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
	@$(MAKE) -C boot clean
	@$(MAKE) -C cpu clean
	@$(MAKE) -C util clean
	@$(MAKE) -C drivers clean
	@$(MAKE) -C kernel clean
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
