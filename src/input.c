#include <input.h>

uint32_t keys[16] = {
  SDLK_x,
  SDLK_1,
  SDLK_2,
  SDLK_3,
  SDLK_q,
  SDLK_w,
  SDLK_e,
  SDLK_a,
  SDLK_s,
  SDLK_d,
  SDLK_z,
  SDLK_c,
  SDLK_4,
  SDLK_r,
  SDLK_f,
  SDLK_v,
};

int keyPressed[16];

int changeKey(SDL_Keycode key, int val) {
  for(int i = 0; i<16; ++i) {
    if(key == keys[i]) {
      keyPressed[i] = val;
      return i;
    }
  }
  return -1;
}
SDL_Keycode getKeyCode(uint32_t key) {
  return keys[key];
}
int pressKey(SDL_Keycode key) {
  return changeKey(key, 1);
}
int releaseKey(SDL_Keycode key) {
  return changeKey(key, 0);
}
void initInput() {
  memset(keyPressed, 0, sizeof(keyPressed));
}
int isKeyPressed(int key) {
  return keyPressed[key];
}
