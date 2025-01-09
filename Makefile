CC = gcc
CFLAGS = -Wall -Wextra -Wuninitialized -g

EXEC = minesweeper
LINK = saper

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

all: $(EXEC) $(LINK)

$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

$(LINK): $(EXEC)
	ln -sf $(EXEC) $(LINK)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

valgrind:
	valgrind --leak-check=full ./$(EXEC) -f data/test

clear:
	rm -f $(OBJ) $(EXEC) $(LINK)