SHELL = /bin/sh

CC=gcc
CFLAGS += -I. -I./include -Wall
CFLAGS += -lglfw -ldl -lm

SRC=main.c src/glad.c every_math.c
OBJS=$(patsubst %.c,%.o, $(SRC))
TARGET=game

.PHONY: all
all: game

$(TARGET) : $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	rm -rf $(TARGET) $(OBJS)
