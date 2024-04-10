TARGET = bin/simpscript
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CFLAGS=-Iinclude

all: clean $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*

$(TARGET): $(OBJ)
	mkdir -p bin
	cc -o $@ $?

obj/%.o: src/%.c
	mkdir -p obj
	cc -c $< -o $@
