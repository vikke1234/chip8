/*
 * =====================================================================================
 *
 *       Filename:  emulateCycle.c
 *
 *    Description:  emulation
 *
 *        Version:  1.0
 *        Created:  07/15/2017 12:55:54 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Viktor Horsmanheimo (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "chip8.h"
#define columnWidth 64

void emulateCycle(){
	c8->opcode = c8->memory[c8->pc] << 8 | c8->memory[c8->pc+1];
	int firstNibble = 0xF000;
	int secondNibble = 0x000F;
	unsigned short positionX = 0;
	unsigned short positionY = 0;

	switch(c8->opcode & firstNibble){
		case 0x0000:
			switch(c8->opcode & secondNibble){
				case 0x0000:
					memset(c8->gfx, 0, sizeof(c8->gfx));
					c8->pc += 2;
					break;
				case 0x000E:
					c8->sp--;
					c8->pc = c8->sp;
					c8->pc += 2;
					break;
			}
			break;

		case 0x1000:
			c8->pc = 0x1000 & 0x0FFF;
			break;
		case 0x2000:
			//double check this
			c8->sp++;
			c8->sp = c8->pc;
			c8->pc = c8->opcode & 0x0FFF;
			break;
		case 0x3000:
			if(c8->V[(c8->opcode & 0x0F00) >> 8] == (c8->opcode & 0x00FF))
				c8->pc+=4;
			else c8->pc += 2;
			break;
		case 0x4000:
			if(c8->V[(c8->opcode & 0x0F00) >> 8] != (c8->opcode & 0x00FF))
				c8->pc += 4;
			else c8->pc += 2;
			break;
		case 0x5000:
			if(c8->V[(c8->opcode & 0x0F00) >> 8] == (c8->opcode & 0x00F0) >> 4)
				c8->pc += 4;
			else c8->pc += 2;
			break;
		case 0x6000:
			c8->V[(c8->opcode & 0x0F00) >> 8] = 
				c8->opcode & 0x00FF;
			break;
		case 0x7000:
			c8->V[(c8->opcode & 0x0F00) >> 8] += 
				c8->opcode & 0x00FF;
			break;
		case 0x8000:
			switch(c8->opcode & 0x000F){
				case 0x0000:
					c8->V[(c8->opcode & 0x0F00)>>8] = 
						c8->V[(c8->opcode) >> 4];
					c8->pc += 2;		
					break;
				case 0x0001:
					c8->V[(c8->opcode & 0x0F00) >> 8] |= 
						c8->V[(c8->opcode & 0x00F0) >> 4];
					c8->pc += 2;
					break;
				case 0x0002:
					c8->V[(c8->opcode & 0x0F00) >> 8] &= 
						c8->V[(c8->opcode & 0x00F0) >> 4];
					c8->pc += 2;
					break;
				case 0x0003:
					c8->V[(c8->opcode & 0x0F00) >> 8] ^= 
						c8->V[(c8->opcode & 0x00F0) >> 4];
					c8->pc += 2;
					break;
				case 0x0004:
					positionX = (c8->opcode & 0x0F00) >> 8;

					c8->V[positionX] += c8->V[(c8->opcode & 0x00F0) >> 4];
					if(c8->V[positionX] > 255)
						c8->V[0xF] = 1;
					c8->pc += 2;
					break;
				case 0x0005:
					positionX = (c8->opcode & 0x0F00) >> 8;
					positionY = (c8->opcode & 0x00F0) >> 4;

					if(c8->V[positionX] > c8->V[positionY])
						c8->V[0xF] = 1;
					else c8->V[0xF] = 0;
					c8->V[positionX] -= c8->V[positionY];
					c8->pc += 2;
					break;
				case 0x0006:
					positionX = (c8->opcode & 0x0F00) >> 8;

					c8->V[0xF] = c8->V[positionX] & 1;
					c8->V[positionX] /= 2;
					c8->pc += 2;
					break;
				case 0x0007:
					positionX = (c8->opcode & 0x0F00) >> 8;
					positionY = (c8->opcode & 0x00F0) >> 4;
					
					if(c8->V[positionX] < c8->V[positionY])
						c8->V[0xF] = 1;
					else c8->V[0xF] = 0;
					c8->V[positionX] =
					   c8->V[positionY] - c8->V[positionX];
					c8->pc += 2;
					break;
				case 0x000E:
					positionX = (c8->opcode & 0x0F00) >> 8;
					c8->V[0xF] = 
						c8->V[positionX] >> 7;
					c8->V[positionX] <<= 1;
					c8->pc+=2;
					break;
			}
		case 0x9000:
			if(c8->V[(c8->opcode & 0x0F00) >> 8] != c8->V[(c8->opcode & 0x00F0) >> 4])
				c8->pc += 4;
			else c8->pc += 2;
			break;
		case 0xA000:
			c8->I = c8->opcode & 0x0FFF;
			c8->pc += 2;
			break;
		case 0xB000:
			c8->pc = (c8->opcode & 0x0FFF) + c8->V[0];
			break;
		case 0xC000:
			c8->V[c8->opcode & 0x0F00] = (rand() % 255) & (c8->opcode & 0x00FF);
			c8->pc += 2;
			break;
		case 0xD000:
			//incomplete, fix this
			;
			c8->V[0xF] = 0;
			int xCoordinate = c8->V[(c8->opcode & 0x00F0) >> 4];
			int yCoordinate = c8->V[(c8->opcode & 0x0F00) >> 8];
			int height = c8->opcode & 0x000F;
			for(int yLine = 0; yLine < height; yLine++){
				for(int xLine = 0; xLine < 8; xLine++){
					int coordinate = yCoordinate + yLine;
						coordinate *= columnWidth;
					   	coordinate += (xCoordinate + xLine);
					if(c8->gfx[coordinate] == 1)
						c8->V[0xF] = 1;
					c8->gfx[coordinate] ^= c8->memory[c8->I + yLine];
				}
			}
			c8->pc += 2;
			break;
		case 0xE000:
			switch(c8->opcode & 0x00F0){
				case 0x0090:
					//fix this
					//if key pressed skip instruction
					
					c8->pc += 2;
					break;
				case 0x00A0:
					//fix this
					//if key not pressed skip next instruction

					c8->pc += 2;
					break;
			}
			break;
		case 0xF000:
			switch(c8->opcode & 0x00FF){
				case 0x0007:
					c8->V[(c8->opcode & 0x0F00)>> 8] = c8->delay_timer;
					c8->pc+=2;
					break;
				case 0x000A:
					//still broken
					;
					SDL_Event e;
					for(int i = 0; i < 16; i++){

					}
						
					c8->pc+=2;
					break;
				case 0x0015:
					c8->delay_timer = (c8->opcode & 0x0F00) >> 8;
					c8->pc += 2;
					break;
				case 0x0018:
					c8->sound_timer = (c8->opcode & 0x0F00) >> 8;
					c8->pc += 2;
					break;
				case 0x001E:
					c8->I += c8->V[(c8->opcode & 0x0F00) >> 8];

			}

		default:
			printf("unknown opcode: %X", c8->opcode);
			c8->pc+=2;
	}
}

