CC = cc
CFLAGS = -Wall -Wextra -Wuninitialized -g
DEBUG = -DDEBUG

EXEC = minesweeper
LINK = saper

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

.PHONY: all objdir debug valgrind test test1 test2 test3 clear clean

all: objdir $(EXEC) $(LINK)
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
$(LINK): $(EXEC)
	ln -sf $(EXEC) $(LINK)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

objdir:
	mkdir -p obj

# debugging
debug:
	$(CC) -o $(EXEC) $(SRC) $(CFLAGS) $(DEBUG)

valgrind:
	valgrind --leak-check=full ./$(EXEC) -f data/test

# tests
test: all
	./$(EXEC) -f data/test

test1: all
	./$(EXEC) -f data/test1

test2: all
	./$(EXEC) -f data/test2

test3: all
	./$(EXEC) -f //\data/test3//

# clear - object files, executable, linker
clear:
	rm -f $(OBJ) $(EXEC) $(LINK)

# clean - whole obj folder with object files, executable, linker
clean:
	rm -rf obj $(EXEC) $(LINK)