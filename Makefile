CC = gcc
CFLAGS = -Wall -Wextra -Wuninitialized -g

EXEC = minesweeper

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, bin/%.o, $(SRC))

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

valgrind:
	valgrind --leak-check=full ./$(EXEC) -f data/test

clear:
	rm -f $(OBJ) $(EXEC)