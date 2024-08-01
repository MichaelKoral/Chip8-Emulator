#include <stdint.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include <chip8.h>
#include <display.h>
#include <input.h>
#include <reader.h>
#include <sound.h>
#include <snake.h>

#define TIMER_RATE 60
#define CLOCK_RATE 800
#define NANOSECONDS 1000000000lu
#define DEBUG 1

void mainLoop() {
  const uint32_t clockCycleTime = NANOSECONDS/CLOCK_RATE;
  struct timespec deltaTime;
  struct timespec previousTime;
  timespec_get(&previousTime, TIME_UTC);

  int running = 1;
  SDL_Event event;
  uint64_t cycleCount = 0;
  const uint32_t cyclesForTimer = CLOCK_RATE/TIMER_RATE;

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
    }
    word instr = loadInstruction();
    if(!DEBUG)
      instr = decodeInstruction(instr);

    struct timespec time;
    timespec_get(&time, TIME_UTC);
    deltaTime.tv_sec = time.tv_sec-previousTime.tv_sec;
    deltaTime.tv_nsec = time.tv_nsec-previousTime.tv_nsec;
    previousTime = time;
    if(deltaTime.tv_nsec < clockCycleTime) {
      nanosleep(&deltaTime, &deltaTime);//doesn't work on windows 
    }
    if(isSoundTimerActive()) {
      playSound();
    }
    if(cycleCount%cyclesForTimer) {
      decrementTimers();
      if(isSoundTimerActive()) {
        printf("test");
      }
    }

    if(!isPaused() && instr != 0x0000) {
      printf("Instruction: %x Cycle: %x\n", instr, (unsigned int)cycleCount);
      fflush(stdout);
      execute(instr);
    }
    render();
    cycleCount++;
  }
}
int main(int argc, char* argv[]) {
  initSDL();
  initDisplay();
  initChip8();
  initSound("sound/beep.wav");

  byte* program = NULL;
  char* path = NULL;
  if(argc > 1) {
    path = argv[1];
  } else if(!DEBUG){
      printf("No path specified");
      return 1;
  }
  uint32_t programSize = 0;
  if(!DEBUG) {
    programSize = readFile(&program, path);
    printf("%s loaded as %d instructions\n", path, programSize);
    fflush(stdout);
  } else {
    program = (byte*)snakeProgram;
    programSize = SNAKE_SIZE;
  }

  loadProgram(program, programSize);

  mainLoop();
  displayCleanup();
  soundCleanup();
  SDLCleanup();
  free(program);
  return 0;
}
