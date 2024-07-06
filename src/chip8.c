#include <stdint.h>

#include <chip8.h>
#include <display.h>
#include <stdlib.h>
#include <time.h>

chipState state;
void init() {
  srand(time(NULL));
  memset(&state, 0, sizeof(chipState));
}
void execute(uint16_t instr) {
  uint16_t addr = ((1<<12)-1) & instr; //lower 3 bytes
  uint16_t N = ((1<<4)-1)<<12 & instr; //higher nibble
  uint16_t n = ((1<<4)-1) & instr; //lower nibble
  uint16_t x = (((1<<4)-1)<<8 & instr)>>8; //lower nibble of high byte
  uint16_t y = (((1<<4)-1)<<4 & instr)>>4; //higher nibble of low byte

  if(instr == 0x00E0) { // 00E0 - CLS Clear the display.
    int pixels[64][32];
    memset(pixels, 0, 64*32*sizeof(int));
    setPixels(pixels);
  }
  else if(instr == 0x00EE) { // 00EE - RET Return from a subroutine.
    state.PC = state.stack[state.SP--];
  }
  else if(N == 1) { // 1nnn - JP addr Jump to location nnn. 
    state.PC = addr;
  }
  else if(N == 2) { // 2nnn - CALL addr Call subroutine at nnn.
    state.stack[state.SP++] = state.PC;
    state.PC = addr;
  }
  else if(N == 3) { //3xkk - SE Vx, byte Skip next instruction if Vx = kk.
    if(state.generalRegs[x] == ((y<<4)+n)) {
      state.PC++;
    }
  }
  else if(N == 4) { // 4xkk - SNE Vx, byte Skip next instruction if Vx != kk.
    if(state.generalRegs[x] != ((y<<4)+n)) {
      state.PC++;
    }
  }
  else if(N == 5) { // 5xy0 - SE Vx, Vy Skip next instruction if Vx = Vy.
    if(state.generalRegs[x] == state.generalRegs[y]) {
      state.PC++;
    }
  }
  else if(N == 6) { // 6xkk - LD Vx, byte Set Vx = kk.
    state.generalRegs[x] = (y<<4)+n;
  }
  else if(N == 7) { // 7xkk - ADD Vx, byte Set Vx = Vx + kk.
    state.generalRegs[x] += (y<<4)+n;
  }
  else if(N == 8) {
    if(n == 0) { // 8xy0 - LD Vx, Vy Set Vx = Vy.
      state.generalRegs[x] = state.generalRegs[y];
    }
    else if(n == 1) { // 8xy1 - OR Vx, Vy Set Vx = Vx OR Vy.
      state.generalRegs[x] |= state.generalRegs[y];
    }
    else if(n == 2) { // 8xy2 - AND Vx, Vy Set Vx = Vx AND Vy.
      state.generalRegs[x] ^= state.generalRegs[y];
    }
    else if(n == 3) { // 8xy3 - XOR Vx, Vy Set Vx = Vx XOR Vy.
      state.generalRegs[x] &= state.generalRegs[y];
    }
    else if(n == 4) { // 8xy4 - ADD Vx, Vy Set Vx = Vx + Vy, set VF = carry.
      uint32_t result = ((int)state.generalRegs[x]) + state.generalRegs[y];
      if(result >= 255) {
        state.generalRegs[0xF] = 1;
      }
      state.generalRegs[x] = ((1<<8)-1) & result;
    }
    else if(n == 5) { // 8xy5 - SUB Vx, Vy Set Vx = Vx - Vy, set VF = NOT borrow.
      uint32_t result = ((uint32_t)state.generalRegs[x]) - state.generalRegs[y];
      if(state.generalRegs[x] > state.generalRegs[y]) {
        state.generalRegs[0xF] = 1;
      }
      state.generalRegs[x] = ((1<<8)-1) & result;
    }
    else if(n == 6) { // 8xy6 - SHR Vx {, Vy} Set Vx = Vx SHR 1.
      if(1 & state.generalRegs[x]) {
        state.generalRegs[0xF] = 1;
      }
      state.generalRegs[x] /= 2;
    }
    else if(n == 7) { // 8xy7 - SUBN Vx, Vy Set Vx = Vy - Vx, set VF = NOT borrow.
      uint32_t result = ((uint32_t)state.generalRegs[y]) - state.generalRegs[x];
      if(state.generalRegs[y] > state.generalRegs[x]) {
        state.generalRegs[0xF] = 1;
      }
      state.generalRegs[x] = ((1<<8)-1) & result;
    }
    else if(n == 14) { // 8xyE - SHL Vx {, Vy} Set Vx = Vx SHL 1.
      if((1<<7) & state.generalRegs[x]) {
        state.generalRegs[0xF] = 1;
      }
      state.generalRegs[x] *= 2;
    }
  }
  else if(N == 9) { // 9xy0 - SNE Vx, Vy Skip next instruction if Vx != Vy.
    if(state.generalRegs[x] != state.generalRegs[y]) {
      state.PC++;
    }
  }
  else if(N == 10) { // Annn - LD I, addr Set I = nnn.
    state.I = addr;
  }
  else if(N == 11) { // Bnnn - JP V0, addr Jump to location nnn + V0.
    state.PC = addr + state.generalRegs[0];
  }
  else if(N == 12) { // Cxkk - RND Vx, byte Set Vx = random byte AND kk.
    state.generalRegs[x] = ((y<<4)+n) & (((uint32_t)rand())%256);
  }
  else if(N == 13) { // Dxyn - DRW Vx, Vy, nibble Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

  }


}
