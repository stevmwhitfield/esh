CC = g++

CFLAGS = -g -Wall

TARGET = esh

esh:
	$(CC) $(CFLAGS) ./src/steven.c -o ./dist/esh

clean:
	rm -f ./dist/esh
