/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  main
 *
 *        Version:  1.0
 *        Created:  07/13/2017 11:05:20 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Viktor Horsmanheimo (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include "chip8.h"

/* TODO:
 * setupGraphics, learn SDL and opengl
 * fix graphics setup function, almost done
 *
 * initialize funcition
 * start making emulation cycle function
 * 
 * */

int main(int argc, char *argv[]){
	if (argc < 2){
		puts("usage: ./a.out game");
		exit(1);
	}
	c8 = malloc(sizeof(CH8));
	if(c8 == NULL){
		printf("could not initialize structure for chip8, exitting");
		exit(1);
	}
	c8->game = argv[1];
	
	SDL_Window *window = NULL;
	SDL_GLContext glcontext = NULL;
	glcontext = setupGraphics(window, glcontext);

	//setupInput();
	draw(renderer);

	initialize(window);
	SDL_Event e;
	SDL_Delay(10000);
	quit(window);
	return 0;
}

