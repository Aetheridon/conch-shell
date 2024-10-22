CC = gcc
CFLAGS = -Wall -Wextra -std=gnu17
TARGET = conch
SRCS = conch.c forkexec.c builtin.c terminal.c
OBJS = $(SRCS:.c=.o)
HEADERS = forkexec.h builtin.h terminal.h

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean