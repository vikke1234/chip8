/*
 * =====================================================================================
 *
 *       Filename:  emulateCycle.c
 *
 *    Description:  emulation
 *
 *        Version:  1.0
 *        Created:  07/15/2017 12:55:54 AM
 *       Revision:  none *       Compiler:  gcc
 *
 *         Author:  Viktor Horsmanheimo (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "chip8.h"

#define columnWidth 64

#define shift4(opcode) (opcode & 0x00F0) >> 4
#define shift8(opcode) (opcode & 0x0F00) >> 8

static void clearScreen();
static void returnFromSubroutine();
static void extract_bits(char *dst, char src);
static void setAddress();
static void goToSubroutine();
static void eqVx();
static void neqVx();
static void eqVxWithShift();
static void setVx();
static void addVx();
static void clearScreen(){
	memset(c8->gfx, 0, sizeof(c8->gfx));
	c8->pc += 2;
}
static void returnFromSubroutine(){
	if(c8->sp)
		c8->sp -= 1;
	c8->pc = c8->stack[c8->sp];
	c8->pc += 2;
}
static void setAddress(){
	c8->pc = c8->opcode & 0x0FFF;
}
static void goToSubroutine(){
	c8->stack[c8->sp] = c8->pc;
	c8->sp += 1;
	c8->pc = c8->opcode & 0x0FFF;

}
static void eqVx(){
	if(c8->V[shift8(c8->opcode)] == (c8->opcode & 0x00FF))
		c8->pc+=4;
	else c8->pc += 2;
}
static void neqVx(){
	if(c8->V[shift8(c8->opcode)] != (c8->opcode & 0x00FF))
		c8->pc += 4;
	else c8->pc += 2;
}
static void eqVxWithShift(){ /* 0x5XY0 */
	if(c8->V[shift8(c8->opcode)] == shift4(c8->opcode))
		c8->pc += 4;
	else c8->pc += 2;
}
static void setVx(){
	c8->V[shift8(c8->opcode)] = 
		c8->opcode & 0x00FF;
	c8->pc += 2;
}
static void addVx(){
	c8->V[shift8(c8->opcode)] += c8->opcode & 0x00FF;
	c8->pc += 2;
}
void emulateCycle(){
	c8->opcode = (unsigned short)(c8->memory[c8->pc] << 8);
	c8->opcode |= (unsigned short)(c8 -> memory[c8->pc + 1]);
	printf("executing opcode: %X at: %X, sp: %d\n", c8->opcode, c8->pc, c8->sp);
	int firstNibble = 0xF000;
	int secondNibble = 0x000F;
	unsigned short positionX = 0;
	unsigned short positionY = 0;
	if(c8->delay_timer)
		c8->delay_timer--;
	if(c8->sound_timer)
		c8->sound_timer--;
	switch(c8->opcode & firstNibble){
		case 0x0000:
			switch(c8->opcode & secondNibble){
				case 0x0000:
					clearScreen();
					break;
				case 0x000E:
					returnFromSubroutine();
					break;
				default:
					printf("unkown opcode: %X\n", c8->opcode);
					break;
			}
			break;

		case 0x1000:
			setAddress();
			break;
		case 0x2000:
			goToSubroutine();
			break;
		case 0x3000:
			eqVx();
			break;
		case 0x4000:
			neqVx();
			break;
		case 0x5000:
			eqVxWithShift();
			break;
		case 0x6000:
			setVx();
			break;
		case 0x7000:
			addVx();
			break;
		case 0x8000:
			switch(c8->opcode & 0x000F){
				case 0x0000:
					c8->V[shift8(c8->opcode)] = 
						c8->V[shift4(c8->opcode)];
					c8->pc += 2;		
					break;
				case 0x0001:
					c8->V[shift8(c8->opcode)] |= 
						c8->V[shift4(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0002:
					c8->V[shift8(c8->opcode)] &= 
						c8->V[shift4(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0003:
					c8->V[shift8(c8->opcode)] ^= 
						c8->V[shift4(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0004:
					positionX = shift8(c8->opcode);

					c8->V[positionX] += c8->V[shift4(c8->opcode)];
					if(c8->V[positionX] > 255)
						c8->V[0xF] = 1;
					c8->pc += 2;
					break;
				case 0x0005:
					positionX = shift8(c8->opcode);
					positionY = shift4(c8->opcode);

					if(c8->V[positionX] > c8->V[positionY])
						c8->V[0xF] = 1;
					else c8->V[0xF] = 0;
					c8->V[positionX] -= c8->V[positionY];
					c8->pc += 2;
					break;
				case 0x0006:
					positionX = shift8(c8->opcode);

					c8->V[0xF] = c8->V[positionX] & 1;
					c8->V[positionX] /= 2;
					c8->pc += 2;
					break;
				case 0x0007:
					positionX = shift8(c8->opcode);
					positionY = shift4(c8->opcode);

					if(c8->V[positionX] < c8->V[positionY])
						c8->V[0xF] = 1;
					else c8->V[0xF] = 0;
					c8->V[positionX] =
						c8->V[positionY] - c8->V[positionX];
					c8->pc += 2;
					break;
				case 0x000E:
					positionX = shift8(c8->opcode);
					c8->V[0xF] = 
						c8->V[positionX] >> 7;
					c8->V[positionX] <<= 1;
					c8->pc+=2;
					break;
				default:
					printf("unkown opcode: %X\n", c8->opcode);
					break;
			}
		case 0x9000:
			if(c8->V[shift8(c8->opcode)] != c8->V[shift4(c8->opcode)])
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
			c8->V[shift8(c8->opcode)] = (rand() % 255) & (c8->opcode & 0x00FF);
			c8->pc += 2;
			break;
		case 0xD000:
			c8->V[0xF] = 0;
			int xCoordinate = c8->V[shift8(c8->opcode)];
			int yCoordinate = c8->V[shift4(c8->opcode)];
			uint8_t height = c8->opcode & 0x000F;
			uint8_t pixel = 0, width = 8;
			for(int yLine = 0; yLine < height; yLine++){

				pixel = c8->memory[c8->I + yLine];

				for(int xLine = 0; xLine < width; xLine++){
					if(!(pixel & (0x80 >> xLine))) continue;
					int coordinate = yCoordinate + yLine;
					coordinate *= columnWidth;
					coordinate += (xCoordinate + xLine);
					if(c8->gfx[coordinate] == 1)
						c8->V[0xF] = 1;

					//c8->gfx[coordinate] ^= 1;
					c8->gfx[coordinate] ^= c8->memory[c8->I + yLine];
					//extract_bits(&c8->gfx[coordinate], c8->memory[c8->I + yLine]);

				}
			}
			c8->pc += 2;
			break;
		case 0xE000:
			switch(c8->opcode & 0x00F0){
				case 0x0090:
					//if key pressed skip instruction
					;
					short key1 = shift8(c8->opcode);
					if(c8->key[key1] == 1)
						c8->pc += 4;
					else c8->pc += 2;
					break;
				case 0x00A0:
					//if key not pressed skip next instruction
					;
					short key2 = shift8(c8->opcode);
					if(c8->key[key2] == 0)
						c8->pc += 4;
					else c8->pc += 2;
					break;
				default:
					printf("unkown opcode: %X\n", c8->opcode);
					break;

			}
			break;
		case 0xF000:
			switch(c8->opcode & 0x00FF){
				case 0x0007:
					c8->V[shift8(c8->opcode)] = c8->delay_timer;
					c8->pc+=2;
					break;
				case 0x000A:
					;
					char keypress = 0;
					for(int i = 0; i <16; i++){
						if(c8->key[i]){
							keypress = 1;
							break;
						}
					}					
					if(!keypress)
						return;
					c8->pc+=2;
					break;
				case 0x0015:
					c8->delay_timer = shift8(c8->opcode);
					c8->pc += 2;
					break;
				case 0x0018:
					c8->sound_timer = shift8(c8->opcode);
					c8->pc += 2;
					break;
				case 0x001E:
					c8->I += c8->V[shift8(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0029:
					c8->I = c8->V[shift8(c8->opcode)] * 0x5;
					c8->pc += 2;
					break;
				case 0x0033:
					positionX = shift8(c8->opcode);

					c8->memory[c8->I] = c8->V[positionX] / 100;
					c8->memory[c8->I + 1] = (c8->V[positionX] / 10) % 10;
					c8->memory[c8->I + 2] = (c8->V[positionX] / 100) % 10;
					c8->pc += 2;
					break;
				case 0x0055:
					positionX = shift8(c8->opcode);
					for(int i = 0; i < positionX; i++){
						c8->memory[c8->I + i] = c8->V[i];
					}
					c8->pc += 2;
					break;
				case 0x0065:
					positionX = shift8(c8->opcode);
					for(int i = 0; i < positionX; i++){
						c8->V[i] = c8->memory[c8->I + i];
					}
					c8->pc += 2;
					break;
			}
			break;

		default:
			printf("unknown opcode: %X\n", c8->opcode);
			break;
	}

}

static void extract_bits(char *dst, char src){
	for(int i = 0; i < 8; i++)
		dst[i] ^= src & (1 << i) ? 0xFF : 0;

}
