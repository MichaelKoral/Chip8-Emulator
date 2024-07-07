#ifndef CHIP8_H
#define CHIP8_H

#define MEMORY_SIZE 0x1000
#define STACK_SIZE 0x10
#define NUMBER_OF_REGISTERS 0x10
#define PROGRAM_START 0x200u
#define SPRITE_STRIDE 5
#include <stdint.h>
typedef uint8_t byte;
typedef uint16_t word;
typedef struct chipState {
  byte memory[MEMORY_SIZE];
  word stack[STACK_SIZE];
  byte generalRegs[NUMBER_OF_REGISTERS];  
  word I; 
  word PC;
  byte DT;
  byte ST;
  byte SP;
} chipState;
void execute(word instr);
void initChip8();
#endif
