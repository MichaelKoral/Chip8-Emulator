#include <stdio.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <chip8.h>
#include <display.h>
#include <input.h>
#include <reader.h>

void mainLoop() {

  int running = 1;
  SDL_Event event;
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
    word instr = loadInstruction();
    printf("Instruction: %x\n", instr);
    fflush(stdout);
    if(instr != 0x0000) {
      printf("%x\n",instr);
      execute(instr);
    }
    render();
  }
}
int main() {
  initDisplay();
  initChip8();
  word instr[5] = {
    0x6004,
    0x6101,
    0x6201,
    0xF029,
    0xD120+SPRITE_STRIDE,
  };
  word* program = NULL;
  const char* path = "test/test_opcode.ch8";
  uint32_t programSize = readFile(&program, path);
  printf("%s loaded as %d instructions\n", path, programSize);
  fflush(stdout);
  loadProgram(program, programSize);
  mainLoop();
  displayCleanup();
  free(program);
  return 0;
}
