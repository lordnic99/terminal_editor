kilo: main.c
	$(CC) main.c -o kilo -Wall -Wextra -pedantic -std=c99
run: kilo
	./kilo
clean: kilo
	rm -rf kilo
