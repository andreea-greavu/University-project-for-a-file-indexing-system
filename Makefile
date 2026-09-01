#GREAVU ANDREEA-MARIA
CC = gcc
CFLAGS = -Wall -Wextra -g

build :main.c functii.c tema2.h
	$(CC) $(CFLAGS) -o search_index main.c functii.c

run:
	./search_index
clean:
	rm -f search_index


	