#ifndef CHIP8_H
#define CHIP8_H

#include <iostream>
using namespace std;


#endif // !CHIP8_H


#ifndef CHIP8_IMPLI
#define CHIP8_IMPLI
//we are here defining a chip8 arch 
//and this is what we will need
//current operations code (instructions to do something)
unsigned short opcode;
//Memory , chip8 arch uses 4096 bytes 
unsigned char memory[4096];
//CPU registes , chip8 has 16 registers of 8bit size
unsigned char V[16];
//index register and program counter registers
unsigned short I;
unsigned short PC;
//this is the memory map that will be used :
//0x000-0x1FF - Chip 8 interpreter (contains font set in emu)
//0x050-0x0A0 - Used for the built in 4x5 pixel font set (0-F)
//0x200-0xFFF - Program ROM and work RAM
unsigned char framebuffer[64*32];
//No hardware / interrupt registers but 2 timer ones
unsigned char delay_timer;
unsigned char sound_timer;
//We need to impliment a stack and its pointer well need it when jumping / calling subroutines
unsigned short stack[16];
unsigned short sp;
//key pad of 16 entries
unsigned char keypad[16];


#endif // !CHIP8_IMPLI
