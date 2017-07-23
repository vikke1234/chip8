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
#include <stdint.h>


/* TODO:
 * setupGraphics, learn SDL and opengl; DONE
 * fix graphics setup function, almost done; DONE
 *
 * initialize funcition; DONE
 * start making emulation cycle function; DONE
 * figure out some way to make the code look better;
 * */
void emulateCycle();

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
	window = setupWindow(window);
	glcontext = setupOpenGL(window, glcontext);

	initialize(window);
	const uint8_t *state = SDL_GetKeyboardState(NULL);
	SDL_Scancode codes[] = {SDL_SCANCODE_0, SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4, 
							SDL_SCANCODE_5, SDL_SCANCODE_6, SDL_SCANCODE_7, SDL_SCANCODE_8, SDL_SCANCODE_9,
							SDL_SCANCODE_A, SDL_SCANCODE_B, SDL_SCANCODE_C, SDL_SCANCODE_D, SDL_SCANCODE_E, 
							SDL_SCANCODE_F
	};
	SDL_GL_SwapWindow(window);
	SDL_Event e;
	draw(window);
	//SDL_GetTicks to get the time
	while (true){
		if(e.type == SDL_QUIT)
			break;
		if(SDL_PollEvent(&e))
			if(e.key.keysym.sym == SDLK_ESCAPE)
				break;
		
		SDL_PumpEvents();
		for(int i = 0; i < sizeof(codes)/ sizeof(SDL_Scancode); i++)
			c8->key[i] = state[codes[i]];
		emulateCycle();	
		draw(window);
	}
	quit(window, glcontext);
	return 0;
}

