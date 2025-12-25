CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGET = scheduling_system
SOURCE = scheduling_system.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET)

clean:
	rm -f $(TARGET) rooms.dat schedules.dat

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
