CC = clang
OBJS = main.c graphics.c init.c chip8.h 
OBJ_NAME = a.out
CFLAGS = -std=c11 -g -Wall -Werror
LFLAGS = -lGL -lSDL2

all:
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) 
run: 
	./$(OBJ_NAME)
