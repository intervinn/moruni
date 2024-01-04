CC=gcc
CFLAGS=

build:
	$(CC) ./src/*.c $(CFLAGS) -o ./bin/main
run: build
	./bin/main