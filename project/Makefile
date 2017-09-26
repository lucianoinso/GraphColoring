TARGET = WinterIsComing
CC = gcc
CFLAGS = -std=c99 -Wall -Werror -Wextra -O3 -DNDEBUG -g

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall -Werror -Wextra -O3 -o $@

clean:
	-rm -f *.o
	-rm -f $(TARGET)
