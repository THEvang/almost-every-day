SHELL = /bin/sh

CC=gcc
CFLAGS += -I.

SRC=main.c
OBJS=$(patsubst %.c,%.o, $(SRC))
TARGET=game

.PHONY: all
all: game

$(TARGET) : $(OBJS)
	$(CC) -o $@ $< $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
