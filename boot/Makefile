# Makefile for simpleOS/boot

all : loader.o

%.o: %.asm
	@$(PRINTF) ' -- Assembling \033[1m$<\033[0m...\n'
	$(NASM) -f elf32 -o $@ $^

clean:
	$(RM) -f *.o
