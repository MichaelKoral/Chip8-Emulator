#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>

#include <display.h>

SDL_Window* window = NULL;
SDL_Surface* surface = NULL;
SDL_Renderer* renderer = NULL;
Pixel* pixels = NULL; 

void initSDL() {
  if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_AUDIO) < 0) {
    perror("ERROR: Could not initialize SDL");
  }
  if((window = SDL_CreateWindow("Chip8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, 0)) == NULL) {
    perror("ERROR: Could not create widow");
  }
  if((renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE)) == NULL) {
    perror("ERROR: Could not create renderer");
  }
  if((surface = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 320, 32, 0, 0, 0, 0)) == NULL) {
    perror("ERROR: Could not create surface");
  } 
}
void initPixels() {
  pixels = (Pixel*) calloc(64 * 32, sizeof(Pixel));
  for(int i = 0; i < 64; ++i) {
    for(int j = 0; j < 32; ++j) {
      Pixel* pixel = &pixels[i + (j * 64)];
      pixel->rect.h = 10;
      pixel->rect.w = 10;
      pixel->rect.x = i * 10;
      pixel->rect.y = j * 10;
      pixel->on = 0;
    }
  }
}
void displayCleanup() {
  free(pixels);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
}
void setPixel(int i, int j, int val) {
      pixels[i+(j*64)].on = val;
}
int getPixel(int i, int j) {
      return pixels[i+(j*64)].on;
}
void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i = 0; i < 64; ++i) {
      for(int j = 0; j < 32; ++j) {        
        Pixel* pixel = &pixels[i + (j * 64)];
        if(pixel->on) {
          SDL_RenderFillRect(renderer, &(pixel->rect));
        } 
      }
    } 
    SDL_RenderPresent(renderer);
}
void initDisplay() {
  initPixels(); 
}
void SDLCleanup() {
  SDL_Quit();
}


