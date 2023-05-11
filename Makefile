CFLAGS=-Wall -Werror

main:
	$(CC) $(CFLAGS) main.c util.c instruction.c ui.c -g -lSDL2

test:
	$(CC) $(CFLAGS) test.c util.c instruction.c
