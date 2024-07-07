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
  word instr[5] = {
    0x6004,
    0x6101,
    0x6201,
    0xF029,
    0xD120+SPRITE_STRIDE,
  };
  int pc = 0;
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
    //read instruction
    //interet
    //display()
    }
    if(pc<5) {
      execute(instr[pc]);
      pc++;
    }
    render();
  }
}
int main() {

  initDisplay();
  initChip8();
  mainLoop();
  displayCleanup();
  return 0;
}
