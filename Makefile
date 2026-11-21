# Makefile para projetos raylib no Linux

CC      = gcc
CFLAGS  = -Wall -std=c11 -I/usr/local/include
LDFLAGS = -L/usr/local/lib
LDLIBS  = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Nome do executável = nome da pasta atual
TARGET  = $(notdir $(CURDIR))

SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all run clean