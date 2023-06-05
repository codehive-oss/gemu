CFLAGS=-Wall -Wextra -Werror -g
LDLIBS=-lSDL2

SRCFILES=util.c instruction.c ui.c

main:
	$(CC) $(CFLAGS) main.c $(SRCFILES) $(LDLIBS) -O0

release:
	$(CC) $(CFLAGS) main.c $(SRCFILES) $(LDLIBS) -O2 -o gemu

test:
	$(CC) $(CFLAGS) test.c $(SRCFILES) $(LDLIBS) -O0 -o test-gemu
