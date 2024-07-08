#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>

void initInput();
int isKeyPressed(int key);
int releaseKey(SDL_Keycode key);
int pressKey(SDL_Keycode key);
SDL_Keycode getKeyCode(uint32_t key);
#endif
