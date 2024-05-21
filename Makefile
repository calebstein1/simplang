TARGET = bin/simplang
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
CC = cc
CFLAGS=-Iinclude

all: clean $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*

$(TARGET): $(OBJ)
	mkdir -p bin
	$(CC) -o $@ $?

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) -c $< -o $@ $(CFLAGS)

install:
	mkdir -p /usr/local/bin
	cp $(TARGET) /usr/local/$(TARGET)

uninstall:
	rm -f /usr/local/$(TARGET)
