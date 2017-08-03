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

#define shift4(opcode) ((opcode & 0x00F0) >> 4)
#define shift8(opcode) ((opcode & 0x0F00) >> 8)

static int firstNibble = 0xF000;
static int secondNibble = 0x000F;
unsigned short positionX = 0;
unsigned short positionY = 0;

static inline void clearScreen();
static inline void returnFromSubroutine();
static inline void setAddress();
static inline void goToSubroutine();
static inline void eqVx();
static inline void neqVx();
static inline void eqVxWithShift();
static inline void setVx();
static inline void addVx();
static void opcode_8000();
static void vXnotEqVy();

void emulateCycle(){
	c8->opcode = (c8->memory[c8->pc] << 8)|c8->memory[c8->pc + 1];
	printf("executing opcode: %X at: %X, sp: %d\n", c8->opcode, c8->pc, c8->sp);
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
			opcode_8000();
			break;
		case 0x9000:
			vXnotEqVy();
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
			uint8_t columnHeight = 32;
			for(int y = 0; y < height; y++){

				pixel = c8->memory[c8->I + y];

				for(int x = 0; x < width; x++){

					if((pixel & (0x80 >> x))!= 0) {
						int realX = (xCoordinate + x) > columnWidth-1 ?
						   (xCoordinate + x) - columnWidth-1 : 
						   (xCoordinate+x) < 0 ? 
						   columnWidth-1 - (xCoordinate+x) : 
						   (xCoordinate+x);

						int realY = (yCoordinate + y) > columnHeight-1 ? 
							(yCoordinate + y) - columnHeight-1 : 
							(yCoordinate+y) < 0 ? 
							columnHeight-1 - (yCoordinate+y) : 
							(yCoordinate+y);

						coordinate = (realY * columnWidth) + (realX);

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
					short key1 = c8->V[shift8(c8->opcode)];
					if(key1 < 16 && c8->key[key1] == 1)
						c8->pc += 4;
					else c8->pc += 2;
					break;
				case 0x00A0:
					//if key not pressed skip next instruction
					;
					short key2 = c8->V[shift8(c8->opcode)];
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
					printf("sound_timer: %d", shift8(c8->opcode));
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

static inline void clearScreen(){
	memset(c8->gfx, 0, sizeof(c8->gfx));
	c8->pc += 2;
}
static inline void returnFromSubroutine(){
	if(c8->sp)
		c8->sp -= 1;
	c8->pc = c8->stack[c8->sp];
	c8->pc += 2;
}
static inline void setAddress(){
	c8->pc = c8->opcode & 0x0FFF;
}
static inline void goToSubroutine(){
	c8->stack[c8->sp] = c8->pc;
	c8->sp += 1;
	c8->pc = c8->opcode & 0x0FFF;

}
static inline void eqVx(){ /* something wrong here maybe */
	if(c8->V[shift8(c8->opcode)] == (c8->opcode & 0x00FF))
		c8->pc+=4;
	else c8->pc += 2;
}
static inline void neqVx(){
	if(c8->V[shift8(c8->opcode)] != (c8->opcode & 0x00FF))
		c8->pc += 4;
	else c8->pc += 2;
}
static inline void eqVxWithShift(){ /* 0x5XY0 */
	if(c8->V[shift8(c8->opcode)] == c8->V[shift4(c8->opcode)])
		c8->pc += 4;
	else c8->pc += 2;
}
static inline void setVx(){
	c8->V[shift8(c8->opcode)] = c8->opcode & 0x00FF;
	c8->pc += 2;
}
static inline void addVx(){
	c8->V[shift8(c8->opcode)] += c8->opcode & 0x00FF;
	c8->pc += 2;
}
static void opcode_8000(){

	switch(c8->opcode & 0x000F){
		case 0x0000:
			c8->V[shift8(c8->opcode)] = 
				c8->V[shift4(c8->opcode)];
			c8->pc += 2;		
			break;
		case 0x0001:
			c8->V[shift8(c8->opcode)] = c8->V[shift8(c8->opcode)] | c8->V[shift4(c8->opcode)];
			c8->pc += 2;
			break;
		case 0x0002:
			c8->V[shift8(c8->opcode)] = c8->V[shift8(c8->opcode)] & c8->V[shift4(c8->opcode)];
			//adds 6 instead of 2 for some reason???
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
			c8->V[positionX] += (c8->V[shift4(c8->opcode)] & 0x00FF);
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
			c8->V[0xF] = 
				c8->V[positionX] >> 7;
			c8->V[positionX] <<= 1;
			c8->pc+=2;
			break;
		default:
			printf("unkown opcode: %X\n", c8->opcode);
			break;
	}
}

static void vXnotEqVy(){
	if(c8->V[shift8(c8->opcode)] != c8->V[shift4(c8->opcode)])
		c8->pc += 4;
	else c8->pc += 2;
}

/* 
   static void extract_bits(unsigned char *dst, char src){
   for(int i = 0; i < 8; i++)
   dst[i] ^= src & (1 << i) ? 0xFF : 0;

   }*/
