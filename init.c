/*
 * =====================================================================================
 *
 *       Filename:  init.c
 *
 *    Description:  initialize game functions
 *
 *        Version:  1.0
 *        Created:  07/14/2017 04:54:59 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Viktor Horsmanheimo (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "main.h"
#include "chip8.h"

bool initialize(SDL_Window *window){
	//clear everything
	puts("initializing...");
	memset(c8->stack, 0, sizeof(c8->stack));
	memset(c8->gfx, 0, sizeof(c8->stack));
	memset(c8->memory, 0, sizeof(c8->memory));
	memset(c8->V, 0, sizeof(c8->V));
	c8-> I = c8->sp = c8->opcode = c8->flag = 0;
	srand(time(NULL));
	c8->pc = 0x200; /* counter starts at 0x200 */

	//load font into memory
	for (int i = 0; i < 80; i++)
		c8->memory[i] = fontset[i];
	c8->sound_timer = c8->delay_timer = 0;	
	
	c8->fp = fopen(c8->game, "rb");
	if(c8->fp == NULL){
		printf("could not open: %s", c8->game);
		return false;
	}
	fseek(c8->fp, 0L, SEEK_END);

	size_t sz = ftell(c8->fp);
	rewind(c8->fp);
	printf("filesize: %lu\n", sz);
	char buffer[sz*2];
	memset(buffer, 0, sz*2);

	size_t bytesRead = fread(buffer, 1, sz, c8->fp);
	if(bytesRead != sz){
		fputs("error reading memory", stderr);
		return false;
	}
	for(int i = 0; i < sz; i++)
		c8->memory[i + 512] = buffer[i];
	puts("done");
	return true;
}
