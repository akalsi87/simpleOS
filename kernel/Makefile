# Makefile for simpleOS/kernel

SRCS = $(wildcard *.c)
HDRS = $(wildcard *.h)

OBJS = $(SRCS:.c=.o)
# List of dependencies defined from list of object files
DEPS := $(OBJS:.o=.d)

all : ${OBJS}

-include $(DEPS)

%.o: %.c
	@$(CC) $(CFLAGS) $(INCL) $(WARN) -c -MM -MF $(patsubst %.o,%.d,$@) $<
	@$(PRINTF) ' -- Compiling \033[1m$<\033[0m...\n'
	$(CC) $(CFLAGS) $(INCL) $(WARN) -c $< -o $@

clean:
	$(RM) -f *.o *.d