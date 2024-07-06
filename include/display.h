#include <SDL2/SDL.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
typedef struct Pixel {
  SDL_Rect rect;
  int on;
} Pixel ;
void main_loop();
