#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <stdio.h>
#include <chip8.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <display.h>
void mainLoop() {

  int running = 1;
  SDL_Event event;
  while(running) {
    while(SDL_PollEvent(&event)) {
      if(event.type == SDL_QUIT) {
        running = 0;
        break;
      } 
      if(event.type == SDL_MOUSEBUTTONDOWN) {
        execute(0x00E0);
        printf("mouseButton");
        fflush(stdout);
      }
      render();
    //read instruction
    //interet
    //display()
    }
  }
}
int main() {

  initDisplay();
  mainLoop();
  displayCleanup();
  return 0;
}
