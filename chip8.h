/*
 * =====================================================================================
 *
 *       Filename:  chip8.h
 *
 *    Description:  header
 *
 *        Version:  1.0
 *        Created:  07/13/2017 11:06:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Viktor Horsmanheimo (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <GL/gl.h>
#include <SDL2/SDL.h>

SDL_GLContext setupOpenGL(SDL_Window *window, SDL_GLContext glcontext);
SDL_Window *setupWindow(SDL_Window * window);
void setupInput();
void quit(SDL_Window *window, SDL_GLContext context);
void initialize(SDL_Window *window);
void draw();

typedef struct{
	char *game;
	FILE *fp;
	unsigned char V[16], 
				  memory[4096], 
				  gfx[64*32];

	unsigned short opcode,
				   I, 
				   pc, 
				   stack[16],
				   sp;

	unsigned char delay_timer;
	unsigned char sound_timer;
	unsigned char key[16];

	

}CH8;

CH8 *c8;
  
static char fontset[80] =
{ 
  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
  0x20, 0x60, 0x20, 0x20, 0x70, // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
}; 
