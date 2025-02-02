#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#include <chip8.h>
#include <display.h>
#include <sprites.h>
#include <input.h>
#include <sound.h>

ChipState state;
uint32_t resumeRegister;
uint32_t programEnd;
void clearPixels();
void displaySprite(uint32_t n, uint32_t x, uint32_t y);
ChipState getState() {
  return state;
}
int isPaused() {
  return state.paused;
}
void resumeChip(int key) {
  state.generalRegs[resumeRegister] = key;
  state.paused = 0;
}
void initChip8() {
  srand(time(NULL));
  memset(&state, 0, sizeof(ChipState));
  memcpy(state.memory, sprites, sizeof(sprites));  
  resumeRegister = 0;
  state.PC = 0x200;
}
void execute(word instr) {
  printf(" Instr Loc: %x\n\n", state.PC);
  fflush(stdout);
  uint16_t addr = ((1<<12)-1) & instr; //lower 3 bytes
  uint16_t N = (((1<<4)-1)<<12 & instr) >> 12; //higher nibble
  uint16_t n = ((1<<4)-1) & instr; //lower nibble
  uint16_t x = (((1<<4)-1)<<8 & instr)>>8; //lower nibble of high byte
  uint16_t y = (((1<<4)-1)<<4 & instr)>>4; //higher nibble of low byte

  if(instr == 0x00E0) { // 00E0 - CLS Clear the display.
    clearPixels();
  }
  else if(instr == 0x00EE) { // 00EE - RET Return from a subroutine.
    state.PC = state.stack[--state.SP];
  }
  else if(N == 0x1) { // 1nnn - JP addr Jump to location nnn. 
    state.PC = addr;
    return;
  }
  else if(N == 0x2) { // 2nnn - CALL addr Call subroutine at nnn.
    state.stack[state.SP++] = state.PC;
    state.PC = addr;
    return;
  }
  else if(N == 0x3) { //3xkk - SE Vx, byte Skip next instruction if Vx = kk.
    if(state.generalRegs[x] == ((y<<4)+n)) {
      state.PC += sizeof(word);
    }
  }
  else if(N == 0x4) { // 4xkk - SNE Vx, byte Skip next instruction if Vx != kk.
    if(state.generalRegs[x] != ((y<<4)+n)) {
      state.PC += sizeof(word);
    }
  }
  else if(N == 0x5) { // 5xy0 - SE Vx, Vy Skip next instruction if Vx = Vy.
    if(state.generalRegs[x] == state.generalRegs[y]) {
      state.PC += sizeof(word);
    }
  }
  else if(N == 0x6) { // 6xkk - LD Vx, byte Set Vx = kk.
    state.generalRegs[x] = (y<<4)+n;
  }
  else if(N == 0x7) { // 7xkk - ADD Vx, byte Set Vx = Vx + kk.
    state.generalRegs[x] += (y<<4)+n;
  }
  else if(N == 0x8) {
    if(n == 0x0) { // 8xy0 - LD Vx, Vy Set Vx = Vy.
      state.generalRegs[x] = state.generalRegs[y];
    }
    else if(n == 0x1) { // 8xy1 - OR Vx, Vy Set Vx = Vx OR Vy.
      state.generalRegs[x] |= state.generalRegs[y];
    }
    else if(n == 0x2) { // 8xy2 - AND Vx, Vy Set Vx = Vx AND Vy.
      state.generalRegs[x] &= state.generalRegs[y];
    }
    else if(n == 0x3) { // 8xy3 - XOR Vx, Vy Set Vx = Vx XOR Vy.
      state.generalRegs[x] ^= state.generalRegs[y];
    }
    else if(n == 0x4) { // 8xy4 - ADD Vx, Vy Set Vx = Vx + Vy, set VF = carry.
      uint32_t result = ((uint32_t)state.generalRegs[x]) + state.generalRegs[y];
      state.generalRegs[x] = ((1<<8)-1) & result;
      state.generalRegs[0xF] = result > 255 ? 1 : 0;
    }
    else if(n == 0x5) { // 8xy5 - SUB Vx, Vy Set Vx = Vx - Vy, set VF = NOT borrow.
      uint32_t result = ((uint32_t)state.generalRegs[x]) - state.generalRegs[y];
      uint8_t vfResult = state.generalRegs[x] >= state.generalRegs[y];
      state.generalRegs[x] = ((1<<8)-1) & result;
      state.generalRegs[0xF] = vfResult;
    }
    else if(n == 0x6) { // 8xy6 - SHR Vx {, Vy} Set Vx = Vx SHR 1.
      uint8_t vfResult = 1 & state.generalRegs[x];
      state.generalRegs[x] >>= 1;
      state.generalRegs[0xF] = vfResult;
    }
    else if(n == 0x7) { // 8xy7 - SUBN Vx, Vy Set Vx = Vy - Vx, set VF = NOT borrow.
      uint32_t result = ((uint32_t)state.generalRegs[y]) - state.generalRegs[x];
      uint8_t vfResult = state.generalRegs[y] >= state.generalRegs[x];
      state.generalRegs[x] = ((1<<8)-1) & result;
      state.generalRegs[0xF] = vfResult;
    }
    else if(n == 0xE) { // 8xyE - SHL Vx {, Vy} Set Vx = Vx SHL 1.
      uint8_t vfResult = ((1<<7) & state.generalRegs[x])>>7;
      state.generalRegs[x] <<= 1;
      state.generalRegs[0xF] = vfResult;
    }
  }
  else if(N == 0x9) { // 9xy0 - SNE Vx, Vy Skip next instruction if Vx != Vy.
    if(state.generalRegs[x] != state.generalRegs[y]) {
      state.PC += sizeof(word);
    }
  }
  else if(N == 0xA) { // Annn - LD I, addr Set I = nnn.
    state.I = addr;
  }
  else if(N == 0xB) { // Bnnn - JP V0, addr Jump to location nnn + V0.
    state.PC = addr + state.generalRegs[0];
    return;
  }
  else if(N == 0xC) { // Cxkk - RND Vx, byte Set Vx = random byte AND kk.
    state.generalRegs[x] = ((y<<4)+n) & (((uint32_t)rand())%256);
  }
  else if(N == 0xD) { // Dxyn - DRW Vx, Vy, nibble Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
    displaySprite(n, state.generalRegs[x], state.generalRegs[y]);
  }
  else if(N == 0xE && y == 0x9) { // Ex9E - SKP Vx Skip next instruction if key with the value of Vx is pressed.
    if(isKeyPressed(state.generalRegs[x])) {
      state.PC += sizeof(word);
    }
  }
  else if(N == 0xE && y == 0xA) { // ExA1 - SKNP Vx Skip next instruction if key with the value of Vx is not pressed.
    if(!isKeyPressed(state.generalRegs[x])) {
      state.PC += sizeof(word);
    }
  }
  else if(N == 0xF) {
    byte lowerByte = (y<<4)+n;
    if(lowerByte == 0x07) { // Fx07 - LD Vx, DT Set Vx = delay timer value. 
      state.generalRegs[x] = state.DT;
    }
    else if(lowerByte == 0x0A) { // Fx0A - LD Vx, K Wait for a key press, store the value of the key in Vx.
      state.paused = 1;
      resumeRegister = x;
    }
    else if(lowerByte == 0x15) { // Fx15 - LD DT, Vx Set delay timer = Vx.
      state.DT = state.generalRegs[x];
    }
    else if(lowerByte == 0x18) { // Fx18 - LD ST, Vx Set sound timer = Vx.
      state.ST = state.generalRegs[x];
    }
    else if(lowerByte == 0x1E) { // Fx1E - ADD I, Vx Set I = I + Vx.
      state.I += state.generalRegs[x];
    }
    else if(lowerByte == 0x29) { // Fx29 - LD F, Vx Set I = location of sprite for digit Vx.
      state.I = SPRITE_STRIDE*state.generalRegs[x];
    }
    else if(lowerByte == 0x33) { // Fx33 - LD B, Vx Store BCD representation of Vx in memory locations I, I+1, and I+2.
      state.memory[state.I] = state.generalRegs[x]/100;
      state.memory[state.I+1] = (state.generalRegs[x]%100)/10;
      state.memory[state.I+2] = state.generalRegs[x]%10;
    }
    else if (lowerByte == 0x55) { // Fx55 - LD [I], Vx Store registers V0 through Vx in memory starting at location I.
      for(int i = 0; i <= x; ++i) {
        state.memory[state.I+i] = state.generalRegs[i];
      }
    }
    else if(lowerByte == 0x65) { // Fx65 - LD Vx, [I] Read registers V0 through Vx from memory starting at location I.
      for(int i = 0; i <= x; ++i) { 
        state.generalRegs[i] = state.memory[state.I+i];
      }
    }
  }
  state.PC += sizeof(word);
}

void clearPixels() {
  for (int i = 0; i < 64; ++i) {
    for (int j = 0; j < 32; ++j) {
      setPixel(i, j, 0);
    }
  }
}

void displaySprite(uint32_t n, uint32_t x, uint32_t y) {
  uint8_t vfResult = 0;
  for(int j = 0; j < n; ++j) {
    byte val = state.memory[state.I+j]; 
    for(int i = 0; i < 8; ++i) { // max sprite row width is 8
      int row = (i+x)%64;
      int col = (j+y)%32;
      int bit = (val & (1<<(7-i)))>>(7-i);
      if(getPixel(row, col) && bit) {
        vfResult = 1;
      }
      setPixel(row, col, getPixel(row, col) ^ bit);
    }
  }
  state.generalRegs[0xF] = vfResult;
}
void loadProgram(byte* code, uint32_t size) {
  memcpy(&state.memory[0x200], code, size); 
  programEnd = 0x200+size; 
}
word loadInstruction() {
  if(programEnd == state.PC) return 0x0000;
  return (word)state.memory[state.PC]+(state.memory[state.PC+1]<<8);
}
void pauseChip() {
  state.paused = 1;
}
word decodeInstruction(word instr) {
  return (instr & 0xFF00) >> 8 | (instr & 0x00FF) << 8;
}
void decrementTimers() {
  if(state.DT>0) 
    state.DT--;
  if(state.ST>0) 
    state.ST--;
}
int isSoundTimerActive() {
  return state.ST;
}
