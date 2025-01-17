CC = cc
CFLAGS = -Wall -Wextra -Wuninitialized -g
DEBUG = -DDEBUG

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

debug:
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS) $(DEBUG)

valgrind:
	valgrind --leak-check=full ./$(EXEC) -f data/test

test:
	./$(EXEC) -f data/test

test1:
	./$(EXEC) -f data/test1

test2:
	./$(EXEC) -f data/test2

test3:
	./$(EXEC) -f data/test3

clear:
	rm -f $(OBJ) $(EXEC) $(LINK)