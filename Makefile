CC = gcc
CFLAGS = -Wall -Wextra -std=c99 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

TARGET = rasterizer
SRC = main.c

.PHONY: all clean # tells make that all and clean are not actual files, just arbitrary targets

all: $(TARGET)

# $@ refers to the value of $(TARGET)
# $^ refers to the value of all of the target's dependencies (in this case $(SRC)
$(TARGET): $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
