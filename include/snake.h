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
  0x1214,

  //sprites:
  //top left 8x1, 1x13
  //when writing, the sprite bytes have to be swapped for some reason
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0x8080,
  0xFF80,
  0x00FC,
  //code

  //load border:

  //left side:
  0xA202,
  0x6000,
  0x6100,
  0xD01F,
  0x610F,
  0xD01F,
  0x611E,
  0xD012,

  //right side:
  0xA202,
  0x603F,
  0x6100,
  0xD01F,
  0x610F,
  0xD01F,
  0x611E,
  0xD012,

  //bottom 
  0xA211,
  0x6001,
  0x611F,
  0xD011,
  0x6009,
  0xD011,
  0x6011,
  0xD011,
  0x6019,
  0xD011,
  0x6021,
  0xD011,
  0x6029,
  0xD011,
  0x6031,
  0xD011,
  0xA212,
  0x6039,
  0xD011,

  //top
  0xA211,
  0x6001,
  0x6100,
  0xD011,
  0x6009,
  0xD011,
  0x6011,
  0xD011,
  0x6019,
  0xD011,
  0x6021,
  0xD011,
  0x6029,
  0xD011,
  0x6031,
  0xD011,
  0xA212,
  0x6039,
  0xD011,

  //init vars
  //draw initial snake
  //calculate apple position
  //get input 
  //draw snake head
  //check if intersecting apple/border
  //if border interesecting or self intersecting jump to end
  //if intersecting apple add 1 to len
  //fetch tail dir from ring buff and update 
  //jump back to mainloop or jump back to calc apple pos apple was eaten

  //v0 = reg for reading in snake dir
  //(v1,v2) = (x,y) of head
  //(v3,v4) = (x,y) of tail
  //(v5, v6) = (x,y) of apple
  //v7 = length of snake
  //v8 = ring buff offset of tail block
  //v9 = dir of head
  //vA = has apple been eaten
  //rest are temp
  //
  //directions: 0->up, 1->right, 2->down, 3->left

  //head instr addr: 0x80
  0x611F,//init x=31
  0x620E,//init y=14
  0x6900,//init dir to up
         
  //tail
  0x631F,//init x=31
  0x640F,//init y=15
  0x6702,//init len=2

  //alloc 256 bytes which holds 
  //dir for each snake block 1 byte per block
  //NOTE this means the max snake size is 256
  0xAF00,//load I=0xF00
  0x6000,//set v0=0
  0xF055,//set head block dir to up
  0xAF01,//load I=0xF01
  0xF055,//set tail block dir to up
  0x6801,//ring buff offset starts at 1
 
  0xA202,//load single pixel
  0xD121,//draw head
  0xD341,//draw tail
         
  //calulate apple pos
  0xC53F,//set v5 to random byte & 0x3F=63d
  0x4500,//if v5=0 then exec next line
  0x6501,//add one to v5
  0x453F,//if v5=-0x3F then exec next line
  0x653E,//set v5=0x62
  //repeat code for y pos of apple
  0xC61F,
  0x4600,
  0x6601,
  0x461F,
  0x661E,
  //load and draw apple as single pixel
  0xA202,
  0xD561,
  0x4F00,//if not drawn on top of snake
  0x12BE,//jump to main loop
  //else
  0xD561,//redraw pixel 
  0x129e,//jump to apple calc

  //*****MAINLOOP*****//
  
  //up
  0x6C05,//load vC as key 'w'
  0xECA1,//if vC is pressed exec next line
  0x6900,//set head dir to up
  //right
  0x6C09,//load vC as key 'd'
  0xECA1,//if vC is pressed exec next line
  0x6901,//set head dir to right
  //down
  0x6C08,//load vC as key 's'
  0xECA1,//if vC is pressed exec next line
  0x6902,//set head dir to down
  //left
  0x6C07,//load vC as key 'a'
  0xECA1,//if vC is pressed exec next line
  0x6903,//set head dir to left


  //change snake head pos
  0x6C01,//set vC=1
  0x4900,//if head dir is up
  0x82C5,//subtract 1 from y of head

  0x4901,//if head dir is right
  0x81C4,//add 1 to x of head
  
  0x4902,//if head dir is down
  0x82C4,//add 1 to y of head

  0x4903,//if head dir is left
  0x81C5,//subtract 1 from x of head
  
  0x6A00,//vA=false (snake didn't reach apple)

  //draw new head pos
  0xA202,//load pixel
  0xD121,//draw head 
  0x4F00,//if didn't collide with other pixel
  0x12FE,//jmp to ring buff calculation
  //else
  0x5150,//if apple x != head x
  0x12FD,//fail, jump to end of program
  0x5260,//if apple y != head y
  0x12FD,//fail, jump to end of program
  //therefore the head collided with the apple
  0x6A01,//set vA=true
  0xD121,//redraw head 
 
  //ring buffer 
  //start at 0xF00
  //grows to 0x1000
  //retrieve tail dir
  //get head offset
  //add new head block
  //remove tail block
  0xAF00,//load addr of ring buff addr loc 0x100
  0x8B80,//set vB to tail offset
  //note underflow is fine on this operation
  //since -1 ~ 0xFF for the ring buffer
  0x8B75,//set vB to tail off-len
         
  //add new head block
  0xAF00,//load start of buffer addr loc 0x11A
  0xFB1E,//load offset of new head
  0x8090,//set v0=dir of head
  0xF055,//write v0 to new head block

  //tail should not be removed if apple was eaten
  0x4A00,//if apple wasn't eaten
  0x1314,//jump to set new tail offset
  //else
  0x7701,//add to snake length
  0x129E,//jump to recalc apple

         
  //set new tail offset
  0x8B80,//set vB=tail offset
  0xAF00,//load ring buff addr
  0x6D01,//set vD=1
  0x8BD5,//vB-=1
  0x88B0,//set offset=vB
  0xAF00,//load start of buffer
  0xF81E,//add tail block offset
  0xF065,//read tail dir
  0x8C00,//write tail dir to temp reg vC
  
  //remove prev snake tail
  0xA202,
  0xD341,//load and remove prev tail
  
  //change snake tail pos
  0x6B01,//set vB=1
  0x4C00,//if tail dir is up
  0x84B5,//subtract 1 from y of tail

  0x4C01,//if tail dir is right
  0x83B4,//add 1 to x of tail
  
  0x4C02,//if tail dir is down
  0x84B4,//add 1 to y of tail 

  0x4C03,//if tail dir is left
  0x83B5,//subtract 1 from x of tail
  
  0x12BE,//jump to main loop

  0x0000,//terminating instruction
};
#endif
