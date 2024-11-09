#ifndef CHIP8_H
#define CHIP8_H

#include <iostream>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define draw_flage() 1

void setup_graphics();
void setup_input();
void init();
void loadprogram();
void decodeopcode(uint16_t opcode);
void update_timers();
void emulate_cycle();
void beep();
void draw_graphics();
void set_keys();

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

void setup_graphics(){
    //set up the framebuffer
    for(int i = 0 ; i < 64*32 ; i++){
        framebuffer[i] = 0;
    }
}
void setup_input(){
    for(int i = 0 ;i < 16 ;i++){
        keypad[i] = 0;
    }
    return;
}
//inits all registers and memory
void init(){
    opcode = 0;
    for(int i = 0 ; i < 4096 ;i++){
        memory[i] = 0;
    }
    I = 0;
    PC = 0;
    for(int i = 0 ; i < 16 ;i++){
        V[i] = 0;
    }
}
void loadprogram(){
    //tobe implimented
    return ;
}
//fetches , decode , executes the opcode
void decodeopcode(uint16_t opcode) {
    // Extract specific parts of the opcode
    uint8_t firstNibble = (opcode & 0xF000) >> 12;
    uint8_t X = (opcode & 0x0F00) >> 8;
    uint8_t Y = (opcode & 0x00F0) >> 4;
    uint8_t N = opcode & 0x000F;
    uint8_t NN = opcode & 0x00FF;
    uint16_t NNN = opcode & 0x0FFF;

    switch (firstNibble) {
        case 0x0: // 0x00E0 and 0x00EE
            switch (opcode) {
                case 0x00E0:
                    std::cout << "Clear the display\n";
                    break;
                case 0x00EE:
                    std::cout << "Return from subroutine\n";
                    break;
                default:
                    std::cout << "Execute machine language routine at address " << NNN << "\n";
                    break;
            }
            break;

        case 0x1: // 0x1NNN - Jump to address NNN
            std::cout << "Jump to address " << NNN << "\n";
            break;

        case 0x2: // 0x2NNN - Call subroutine at NNN
            std::cout << "Call subroutine at address " << NNN << "\n";
            break;

        case 0x3: // 0x3XNN - Skip next instruction if VX == NN
            std::cout << "Skip next instruction if V[" << +X << "] == " << +NN << "\n";
            break;

        case 0x4: // 0x4XNN - Skip next instruction if VX != NN
            std::cout << "Skip next instruction if V[" << +X << "] != " << +NN << "\n";
            break;

        case 0x5: // 0x5XY0 - Skip next instruction if VX == VY
            std::cout << "Skip next instruction if V[" << +X << "] == V[" << +Y << "]\n";
            break;

        case 0x6: // 0x6XNN - Set VX to NN
            std::cout << "Set V[" << +X << "] to " << +NN << "\n";
            break;

        case 0x7: // 0x7XNN - Add NN to VX
            std::cout << "Add " << +NN << " to V[" << +X << "]\n";
            break;

        case 0x8: // 0x8XYN - Various bitwise and arithmetic operations
            switch (N) {
                case 0x0:
                    std::cout << "Set V[" << +X << "] to V[" << +Y << "]\n";
                    break;
                case 0x1:
                    std::cout << "Set V[" << +X << "] |= V[" << +Y << "]\n";
                    break;
                case 0x2:
                    std::cout << "Set V[" << +X << "] &= V[" << +Y << "]\n";
                    break;
                case 0x3:
                    std::cout << "Set V[" << +X << "] ^= V[" << +Y << "]\n";
                    break;
                case 0x4:
                    std::cout << "Add V[" << +Y << "] to V[" << +X << "] with carry\n";
                    break;
                case 0x5:
                    std::cout << "Subtract V[" << +Y << "] from V[" << +X << "] with borrow\n";
                    break;
                case 0x6:
                    std::cout << "Right shift V[" << +X << "]\n";
                    break;
                case 0x7:
                    std::cout << "Set V[" << +X << "] = V[" << +Y << "] - V[" << +X << "]\n";
                    break;
                case 0xE:
                    std::cout << "Left shift V[" << +X << "]\n";
                    break;
                default:
                    std::cout << "Unknown opcode\n";
            }
            break;

        case 0xA: // 0xANNN - Set I to address NNN
            std::cout << "Set I to address " << NNN << "\n";
            break;

        case 0xB: // 0xBNNN - Jump to address NNN + V0
            std::cout << "Jump to address " << NNN << " + V0\n";
            break;

        case 0xC: // 0xCXNN - Set VX = random byte AND NN
            std::cout << "Set V[" << +X << "] to random byte AND " << +NN << "\n";
            break;

        case 0xD: // 0xDXYN - Draw sprite
            std::cout << "Draw sprite at (V[" << +X << "], V[" << +Y << "]) with height " << +N << "\n";
            break;

        case 0xE: // Key operations
            if (NN == 0x9E)
                std::cout << "Skip next instruction if key V[" << +X << "] is pressed\n";
            else if (NN == 0xA1)
                std::cout << "Skip next instruction if key V[" << +X << "] is not pressed\n";
            break;

        case 0xF: // Various operations
            switch (NN) {
                case 0x07:
                    std::cout << "Set V[" << +X << "] to delay timer value\n";
                    break;
                case 0x15:
                    std::cout << "Set delay timer to V[" << +X << "]\n";
                    break;
                case 0x1E:
                    std::cout << "Add V[" << +X << "] to I\n";
                    break;
                case 0x29:
                    std::cout << "Set I to location of sprite for digit V[" << +X << "]\n";
                    break;
                case 0x33:
                    std::cout << "Store BCD of V[" << +X << "] in memory\n";
                    break;
                case 0x65:
                    std::cout << "Read registers V0 to V[" << +X << "]\n";
                    break;
                default:
                    std::cout << "Unknown opcode\n";
            }
            break;

        default:
            std::cout << "Unknown opcode\n";
    }
}
void emulate_cycle(){
    //fetch : we need to get the opcode which is stored in 
    //memory (the array) , we get the opcode from the PC
    //register .
    opcode = memory[PC] << 8 | memory[PC+1];
    //decode : well look up the meaning of each opcode in 
    //this switch statement
    
}
void update_timers(){
    if(delay_timer > 0){
        delay_timer --;
    }
    if(sound_timer > 0){
        sound_timer --;
        if(sound_timer == 0) beep();
    }
}
void beep(){
    //replace this with sound code
    return ;
}
void draw_graphics(){
    return ;
}
void set_keys(){
    return;
}
#endif // !CHIP8_IMPLI
