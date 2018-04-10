OBJS = $(wildcard src/*.c)

CC = clang

LFLAGS = -lSDL2

CFLAGS = -Wall

BIN = maze

all : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(BIN) $(LFLAGS)
