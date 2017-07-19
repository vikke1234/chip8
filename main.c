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
#include "chip8.h"
#include <stdbool.h>

/* TODO:
 * setupGraphics, learn SDL and opengl
 * fix graphics setup function, almost done
 *
 * initialize funcition
 * start making emulation cycle function
 * 
 * */

int main(int argc, char *argv[]){
	int width = 64;
	int height = 32;


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
	window = setupWindow(window);
	glcontext = setupOpenGL(window, glcontext);

	initialize(window);
	for(int i = 0; i < 32; i+=2){
		c8->gfx[i] = 255;	
	}
	glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, c8->gfx);
	SDL_GL_SwapWindow(window);
	SDL_Event e;
	while (true){
		if(e.type == SDL_QUIT)
			break;
		if(SDL_PollEvent(&e))
			if(e.key.keysym.sym == SDLK_ESCAPE)
				break;
		glDrawPixels(width, height, GL_LUMINANCE, GL_UNSIGNED_BYTE, c8->gfx);
		SDL_GL_SwapWindow(window);
	}
	quit(window, glcontext);
	return 0;
}

