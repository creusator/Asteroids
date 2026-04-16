CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -g -Iinclude $(shell pkg-config --cflags sdl3 sdl3-ttf)
LDFLAGS = $(shell pkg-config --libs sdl3 sdl3-ttf) -lm
TARGET  = asteroids
SRC     = $(wildcard src/*.c)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
