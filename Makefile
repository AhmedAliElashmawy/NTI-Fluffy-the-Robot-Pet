CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
TARGET = app
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)

.PHONY: all clean
