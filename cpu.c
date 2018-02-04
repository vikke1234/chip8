/*
 * =====================================================================================
 *
 *       Filename:  emulateCycle.c
 *
 *    Description:  cpu emulation
 *
 *        Version:  1.0
 *        Created:  07/15/2017 12:55:54 AM
 *       Revision:  none
 *       Compiler:  clang
 *
 *         Author:  Viktor Horsmanheimo
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "chip8.h"

#define columnWidth 64


static int firstNibble = 0xF000;
static int secondNibble = 0x000F;
unsigned short positionX = 0;
unsigned short positionY = 0;

static int shift4(int opcode);
static int shift8(int opcode);
static int FF(int opcode);
static int FFF(int opcode);
static int get_coordinate(int coordinate, int location);


void emulateCycle(){
	c8->opcode = (c8->memory[c8->pc] << 8) | c8->memory[c8->pc + 1];
	printf("executing opcode: %X at: %X, sp: %d\n", c8->opcode, c8->pc, c8->sp);
	switch(c8->opcode & firstNibble){
		case 0x0000:
			switch(c8->opcode & secondNibble){
				case 0x0000:
					memset(c8->gfx, 0, sizeof(c8->gfx));
					c8->pc += 2;
					break;
				case 0x000E:
					if(c8->sp)
						c8->sp -= 1;
					c8->pc = c8->stack[c8->sp];
					c8->pc += 2;
					break;
				default:
					printf("unkown opcode: %X\n", c8->opcode);
					break;
			}
			break;

		case 0x1000:
			c8->pc = FFF(c8->opcode);
			break;
		case 0x2000:
			c8->stack[c8->sp] = c8->pc;
			c8->sp += 1;
			c8->pc = FFF(c8->opcode);
			break;
		case 0x3000:
			;
			int entry = shift8(c8->opcode);
			if(c8->V[entry] == FF(c8->opcode))
				c8->pc+=4;
			else c8->pc += 2;
			break;
		case 0x4000:
			if(c8->V[shift8(c8->opcode)] != FF(c8->opcode))
				c8->pc += 4;
			else c8->pc += 2;
			break;
		case 0x5000:
			if(c8->V[shift8(c8->opcode)] == c8->V[shift4(c8->opcode)])
				c8->pc += 4;
			else c8->pc += 2;
			break;
		case 0x6000:
			c8->V[shift8(c8->opcode)] = FF(c8->opcode);
			c8->pc += 2;
			break;
		case 0x7000:
			c8->V[shift8(c8->opcode)] += FF(c8->opcode);
			c8->pc += 2;
			break;
		case 0x8000:
			switch(c8->opcode & secondNibble){
				case 0x0000:
					c8->V[shift8(c8->opcode)] = c8->V[shift4(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0001:
					c8->V[shift8(c8->opcode)] = c8->V[shift8(c8->opcode)] | c8->V[shift4(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0002:
					c8->V[shift8(c8->opcode)] = c8->V[shift8(c8->opcode)] & c8->V[shift4(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0003:
					c8->V[shift8(c8->opcode)] ^=
						c8->V[shift4(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0004:
					positionX = shift8(c8->opcode);

					if(c8->V[positionX] + c8->V[shift4(c8->opcode)] > 255)
						c8->V[0xF] = 1;
					else c8->V[0xF] = 0;
					c8->V[positionX] += FF(c8->V[shift4(c8->opcode)]);
					c8->pc += 2;
					break;
				case 0x0005:
					positionX = shift8(c8->opcode);
					positionY = shift4(c8->opcode);

					if(c8->V[positionX]  >  c8->V[positionY])
						c8->V[0xF] = 1;
					else c8->V[0xF] = 0;
					c8->V[positionX] -= c8->V[positionY];
					c8->pc += 2;
					break;
				case 0x0006:
					positionX = shift8(c8->opcode);

					c8->V[0xF] = c8->V[positionX] & 0xFE;
					c8->V[positionX] >>= 1;
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
					c8->V[0xF] = c8->V[positionX] >> 7;
					c8->V[positionX] <<= 1;
					c8->pc+=2;
					break;
				default:
					printf("unkown opcode: %X\n", c8->opcode);
					break;
			}
			break;
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
			c8->V[shift8(c8->opcode)] = (rand() % 256) & (c8->opcode & 0x00FF);
			c8->pc += 2;
			break;
		case 0xD000:
			c8->V[0xF] = 0;
			int xCoordinate = c8->V[shift8(c8->opcode)];
			int yCoordinate = c8->V[shift4(c8->opcode)];
			uint8_t height = c8->opcode & 0x000F;
			uint8_t pixel = 0, width = 8;
			int coordinate = 0;
			for(int y = 0; y < height; y++){

				pixel = c8->memory[c8->I + y];

				for(int x = 0; x < width; x++){

					if((pixel & (0x80 >> x)) != 0) {
						int realX = get_coordinate(xCoordinate, x);
						int realY = get_coordinate(yCoordinate, y);

						coordinate = realY * columnWidth + realX;

						if(c8->gfx[coordinate] == 255)
							c8->V[0xF] = 1;

						c8->gfx[coordinate] ^= 255;
					}
				}
			}
			c8->flag = 1;
			c8->pc += 2;
			break;
		case 0xE000:
			switch(c8->opcode & 0x00F0){
				case 0x0090:
					//if key pressed skip instruction
					;
					short key1 = c8->V[shift8(c8->opcode) & 0xF];
					if(key1 < 16 && c8->key[key1] == 1)
						c8->pc += 4;
					else c8->pc += 2;
					break;
				case 0x00A0:
					//if key not pressed skip next instruction
					;
					short key2 = c8->V[shift8(c8->opcode) & 0xF];
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
							c8->V[shift8(c8->opcode)] = i;
							break;
						}
					}
					if(!keypress)
						return;
					c8->pc+=2;
					break;
				case 0x0015:
					c8->delay_timer = c8->V[shift8(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0018:
					c8->sound_timer = c8->V[shift8(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x001E:
					c8->I += c8->V[shift8(c8->opcode)];
					c8->pc += 2;
					break;
				case 0x0029:
					c8->I = c8->V[shift8(c8->opcode)] * 5;
					c8->pc += 2;
					break;
				case 0x0033:
					positionX = shift8(c8->opcode);

					c8->memory[c8->I] = c8->V[positionX] / 100;
					c8->memory[c8->I + 1] = (c8->V[positionX] / 10) % 10;
					c8->memory[c8->I + 2] = c8->V[positionX]  % 10;
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
					for(int i = 0; i <= positionX; i++){
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

	if(c8->delay_timer)
		c8->delay_timer--;
	if(c8->sound_timer)
		c8->sound_timer--;
}

static int shift4(int opcode){
	return (opcode & 0x00F0) >> 4;
}

static int shift8(int opcode){
	return (opcode & 0x0F00) >> 8;
}

static int FF(int opcode){
	return opcode & 0x00FF;
}
static int FFF(int opcode){
	return opcode & 0x0FFF;
}

static int get_coordinate(int coordinate, int location){
	return (coordinate + location ) >  columnWidth - 1 ?
		( coordinate + location ) - columnWidth - 1 :
		(coordinate + location) < 0 ?
		(columnWidth - 1) - (coordinate + location) :
		(coordinate + location);

}
