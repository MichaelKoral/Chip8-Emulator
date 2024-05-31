#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <chip8.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

int main() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
    perror("ERROR: Could not initialize SDL");
  }
  if (SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0) < 0) {
    perror("ERROR: Could not create widow");
  }
  while (1) {}
  return 0;
}
