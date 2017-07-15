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
	c8->opcode = memory[pc] << 8 | memory[pc+1];
	
	switch(c8->opcode & 0xF000){
		case 0x0000:
			switch(c8->opcode & 0x000F){
				case 0x0000:
					memset(c8->gfx, 0, sizeof(gfx));
					pc += 2;
					break;
				case 0x000E:

					break;
			}
			break;

		case 0x1000:

			break;
		default:
			printf("unknown opcode: %X", c8->opcode);
			pc+=2;
	}
}

