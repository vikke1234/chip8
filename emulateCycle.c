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
#include "chip8.h"

//figure this hsit out
void emulateCycle(){
	c8->opcode = c8->memory[c8->pc] << 8 | c8->memory[c8->pc+1];
	int firstNibble = 0xF000;
	int secondNibble = 0x000F;

	switch(c8->opcode & firstNibble){
		case 0x0000:
			switch(c8->opcode & secondNibble){
				case 0x0000:
					memset(c8->gfx, 0, sizeof(c8->gfx));
					c8->pc += 2;
					break;
				case 0x000E:

					break;
			}
			break;

		case 0x1000:
			c8->pc = 0x1000 & 0x0FFF;
			break;
		case 0x2000:
			c8->sp++;
			c8->sp = c8->pc;
			c8->pc = 0x2000 & 0x0FFF;
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
			c8->V[(c8->opcode & 0x0F00) >> 8] = c8->opcode & 0x00FF;
			break;
		case 0x7000:
			c8->V[(c8->opcode & 0x0F00) >> 8] += c8->opcode & 0x00FF;
			break;
		case 0x8000:
			switch(c8->opcode & 0x000F){
				case 0x0000:
					c8->V[(c8->opcode & 0x0F00)>>8] = c8->V[(c8->opcode) >> 4];
					c8->pc += 2;		
					break;
				case 0x0001:
					c8->V[(c8->opcode & 0x0F00) >> 8] |= c8->V[(c8->opcode & 0x00F0) >> 4];
					c8->pc += 2;
					break;
				case 0x0002:
					c8->V[(c8->opcode & 0x0F00) >> 8] &= c8->V[(c8->opcode & 0x00F0) >> 4];
					c8->pc += 2;
					break;
				case 0x0003:
					c8->V[(c8->opcode & 0x0F00) >> 8] ^= c8->V[(c8->opcode & 0x00F0) >> 4];
					c8->pc += 2;
					break;
				case 0x0004:
					unsigned short position = position =(c8->opcode & 0x0F00) >> 8;
				    
					c8->V[position] += c8->V[(c8->opcode & 0x00F0) >> 4];
					if(c8->V[position] > 255)
						c8->V[0xF] = 1;
						c8->pc += 2;
					break;
				case 0x0005:
					unsigned short positionX = 0;
					unsigned short positionY = 0;
					positionX = (c8->opcode & 0x0F00) >> 8;
					positionY = (c8->opcode & 0x00F0) >> 4;

					if(c8->V[positionX] > c8->V[positionY])
						c8->V[0xF] = 1;
					else c8->V[0xF] = 0;
					c8->V[positionX] -= c8->V[positionY];
					c8->pc += 2;
					break;
				case 0x0006:
					unsigned short position = (c8->opcode & 0x0F00) >> 8;
					c8->V[0xF] = c8->V[position] & 1;
					c8->V[position] /= 2;
					c8->pc += 2;
					break;
				case 0x0007:
					unsigned short positionX = (c8->opcode & 0x0F00) >> 8;


			}
		default:
			printf("unknown opcode: %X", c8->opcode);
			pc+=2;
	}
}

