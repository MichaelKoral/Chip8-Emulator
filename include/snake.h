#ifndef SNAKE_H
#define SNAKE_H
#include <chip8.h>
#define SNAKE_SIZE 1000
// 64x32
// load border
// snake moves foward direction(input changes dir)
// if snake hits apple then add a block to tail (store its pos)continue to move func
// if drawing results in flag then it intersected border or itself -> so fail
// move func: remove last block (it should be stored)
// 
word snakeProgram[SNAKE_SIZE] = {
  //jump to start of code
  0x1222,

  //sprites:
  //top left 8x1, 1x13
  0x80FF,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  //mid left 1x14 
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  //bottom left 1x3 8x1
  0x8080,
  0xFF80,


  //code
  //load border
  0xA202,
  0x6000,
  0x6100,
  0xD01E,

  0xA210,
  0x6000,
  0x610E,
  0xD01E,

  0xA21E,
  0x6000,
  0x611C,
  0xD014,
};
#endif
