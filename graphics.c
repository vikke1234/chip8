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

/*if you just need to blit some pixels
 glPixelZoom, glPixelStore*, glViewPort, glRasterPos, glDrawPixels
 PixelZoom and RasterPos set up the draw orientation on the screen and how it's flipped
 ViewPort adjusts the sub-space of the window in which things are drawn
 PixelStore lets the GPU know what the alignment of your data is
 DrawPixels sends your data to the GPU and uses the set parameters to place it in the frame buffer
 you usually set the zoom and storing once. change the viewport if your window changes size
  */
SDL_Window *setupWindow(SDL_Window *window){
	int width = 64;
	int height = 32;
	int zoom = 10;
	int w_zoom = width * zoom;
	int h_zoom = height * zoom;

	if(SDL_Init(SDL_INIT_EVERYTHING)<0){
		printf("could not initialize SDL: %s\n", SDL_GetError());	
		exit(1);
	}
	if((window = SDL_CreateWindow("CHIP 8 Emulator", 
					SDL_WINDOWPOS_UNDEFINED, 
					SDL_WINDOWPOS_UNDEFINED, 
					w_zoom, h_zoom, 
					SDL_WINDOW_OPENGL ))<0){

		printf("could not create window: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_SetWindowSize(window, w_zoom, h_zoom);

	return window;
}
SDL_GLContext setupOpenGL(SDL_Window *window, SDL_GLContext glcontext){
	int zoom = 10;

	glcontext = SDL_GL_CreateContext(window);

	if(glcontext == NULL){
		printf("could not initialize glcontext: %s", SDL_GetError());
		quit(window, NULL);
	}
	glPixelZoom(zoom, -zoom);
	glRasterPos2i(-1, 1);
	SDL_GL_SetSwapInterval(1);

	return glcontext;
	
}

void quit(SDL_Window *window, SDL_GLContext context){
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	exit(1);
}

void draw(SDL_Window *window){
	short width = 64;
	short height = 32;

	glDrawPixels(width, 
				height, 
				GL_LUMINANCE, 
				GL_UNSIGNED_BYTE, 
				c8->gfx);
	SDL_GL_SwapWindow(window);
}
