#include <stdio.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <chip8.h>
#include <display.h>
#include <input.h>

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
      if(event.type == SDL_KEYDOWN) {
        int key = pressKey(event.key.keysym.sym);
        if(isPaused() && key != -1) {
          resumeChip(key);
        }

      }
      if(event.type == SDL_KEYUP) {
        releaseKey(event.key.keysym.sym);      
      }
      if(event.type == SDL_MOUSEBUTTONDOWN) {
        execute(0x00E0);
        printf("mouseButton");
        fflush(stdout);
      }
    }
    if(isPaused()) {
      continue;
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
