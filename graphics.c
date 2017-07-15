/*
 * =====================================================================================
 *
 *       Filename:  graphics.c
 *
 *    Description:  functions for setting up graphics and drawing
 *
 *        Version:  1.0
 *        Created:  07/13/2017 11:22:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Viktor Horsmanheimo (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include "chip8.h"

SDL_GLContext setupGraphics(SDL_Window *window, SDL_GLConext *glcontext){
	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		printf("could not initialize SDL: %s\n", SDL_GetError());	
		exit(1);
	}
	if(SDL_CreateWindow("CHIP 8 Emulator", 
				SDL_WINDOWPOS_UNDEFINED, 
				SDL_WINDOWPOS_UNDEFINED,
				64, 32, 
				SDL_WINDOW_OPENGL )){

		printf("could not create window: %s\n", SDL_GetError());
		exit(1);
	}
	glcontext = SDL_GL_CreateConext(window);
	return glcontext;
	
}

void quit(SDL_Window *window){
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void draw(SDL_Renderer *renderer){
	puts("drawing");
    SDL_RenderSetLogicalSize( renderer, 500, 500 );
	for(int i; i<32;i++)
		for(int j; j < 16; j++)
			SDL_RenderDrawPoint(renderer, i, j); /* incomplete */
	SDL_RenderPresent(renderer);
}
