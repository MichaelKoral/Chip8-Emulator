#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
typedef struct Pixel {
  SDL_Rect rect;
  int on;
} Pixel ;

void initDisplay();
void render();
void displayCleanup();
void setPixel(int i, int j, int val);
int getPixel(int i, int j);

#endif
