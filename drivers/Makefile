# Makefile for simpleOS/drivers

SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)

OBJS = $(SRCS:.c=.o)
# List of dependencies defined from list of object files
DEPS := $(OBJS:.o=.d)

all : ${OBJS}

-include $(DEPS)

%.o: %.asm
	@$(PRINTF) 'Assembling \033[1m$<\033[0m...\n    '
	$(NASM) -f elf32 -o $@ $^

%.o: %.c
	@$(ECHO) 'Compiling \033[1m$<\033[0m...'
	@$(CC) $(CFLAGS) $(INCL) $(WARN) -c -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(PRINTF) '    '
	$(CC) $(CFLAGS) $(INCL) $(WARN) -c $< -o $@
	@$(ECHO)


clean:
	$(RM) -f *.o *.d
