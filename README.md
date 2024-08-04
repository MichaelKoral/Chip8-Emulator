Simple chip8 emulator and snake rom written in c

# Building and Running
Just run make

To run the binary do

`./bin/chip8 <chip8 program>`

# Snake
Snake source code is in snake.h, the rom snake.ch8 is in the main directory

The controls are just wasd on a standard keyboard or 5,7,8,9 on the chip8 keyboard

I run it at 180-240 for clock speed, I haven't tested it with anything else

There are couple of things missing from it
- Sound when the snake eats an apple
- A title screen
- A losing screen
- A score counter
- A win condition


Reference used:
  http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
