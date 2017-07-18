CC = gcc
OBJS = main.c chip8.h graphics.c init.c 
OBJ_NAME = a.out
CFLAGS = -g
LFLAGS = -lGL -lSDL2

all:
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) -o $(OBJ_NAME)
run: 
	./$(OBJ_NAME)
