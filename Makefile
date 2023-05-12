CFLAGS=-Wall -Werror

main:
	$(CC) $(CFLAGS) main.c util.c instruction.c ui.c -g -O0 -lSDL2

release:
	$(CC) $(CFLAGS) main.c util.c instruction.c ui.c -lSDL2 -g -O2 -o gemu

test:
	$(CC) $(CFLAGS) test.c util.c instruction.c
