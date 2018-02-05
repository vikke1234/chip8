CC = clang
OBJS = main.c cpu.c graphics.c init.c chip8.h 
OBJ_NAME = a.out
CFLAGS = -std=c11 -g -Wall -fsanitize=address -fno-omit-frame-pointer -O2 -ggdb
LFLAGS = -lGL -lSDL2

all:
	$(CC) $(CFLAGS) $(LFLAGS) $(OBJS) 
run: 
	./$(OBJ_NAME)
