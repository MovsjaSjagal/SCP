CC = gcc
CFLAGS = -O2 -Wall

all: program

program: src/main.c src/matrix_io.c
	$(CC) $(CFLAGS) src/main.c src/matrix_io.c -o program

clean:
	rm -f program
