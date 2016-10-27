# Makefile for simpleOS/boot

all : loader.o

loader.o: loader.asm
	$(NASM) -f elf32 -o $@ $^

clean:
	$(RM) -f *.o
