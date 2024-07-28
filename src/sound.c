#include <SDL2/SDL_audio.h>
#include <stdint.h>

uint8_t* wavBuff;
uint8_t* wavPos;
uint32_t wavLen;
uint32_t fullLen;
int playingSound = 0;

void playAudioCallback(void* userData, uint8_t* stream, int len);
void initSound(const char* path) {
  SDL_AudioSpec spec;

  if(SDL_LoadWAV(path, &spec, &wavBuff, &wavLen) == NULL) {
    printf("Error loading wav: %s", path); 
    fflush(stdout);
    exit(1);
  }
  
  spec.callback = playAudioCallback; 
  if(SDL_OpenAudio(&spec, NULL) < 0) {
    printf("Error opening audio");
    fflush(stdout);
    exit(1);
  }
  wavPos = wavBuff;
  fullLen = wavLen;
}
void playAudioCallback(void* userData, uint8_t* stream, int len) {
  if(wavLen <= 0) {
    SDL_PauseAudio(1);
    playingSound = 0;
    wavPos = wavBuff;
    wavLen = fullLen;
    return;
  }

  len = len > wavLen ? wavLen : len;
  SDL_memcpy(stream, wavPos, len); 

  wavPos += len;
  wavLen -= len;
}

void playSound() {
  if(playingSound) 
    return;
  playingSound = 1;
  SDL_PauseAudio(0);
}

void soundCleanup() {
  SDL_CloseAudio();
  SDL_FreeWAV(wavBuff);
}
