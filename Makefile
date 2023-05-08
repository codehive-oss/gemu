CFLAGS=-Wall -Werror

main:
	$(CC) $(CFLAGS) main.c util.c instruction.c -g
