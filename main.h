/*
 * =====================================================================================
 *
 *       Filename:  main.h
 *
 *    Description:  header for main.c
 *
 *        Version:  1.0
 *        Created:  28/07/17 19:18:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <GL/gl.h>

void quit(SDL_Window *window, SDL_GLContext context);
void initialize(SDL_Window *window);
void draw();
SDL_GLContext setupOpenGL(SDL_Window *window, SDL_GLContext glcontext);
SDL_Window *setupWindow(SDL_Window * window);
