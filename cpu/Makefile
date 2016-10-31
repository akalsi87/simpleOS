# Makefile for simpleOS/cpu

ASMSRCS = $(wildcard *.asm)
SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)

OBJS = $(SRCS:.c=.o)
ASMOBJS = $(ASMSRCS:.asm=.o)
# List of dependencies defined from list of object files
DEPS := $(OBJS:.o=.d)

all : ${OBJS} ${ASMOBJS}
-include $(DEPS)

%.o: %.asm
	@$(PRINTF) ' -- Assembling \033[1m$<\033[0m...\n'
	$(NASM) -f elf32 -o $@ $^

%.o: %.c
	@$(CC) $(CFLAGS) $(INCL) $(WARN) -c -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(PRINTF) ' -- Compiling  \033[1m$<\033[0m...\n'
	$(CC) $(CFLAGS) $(INCL) $(WARN) -c $< -o $@

clean:
	$(RM) -f *.o *.d
