#include <iostream>
#include <string>
#include "SFML/Window.hpp"
#include <stdio.h>
#include <stdlib.h>  

class chip8
{
    unsigned short instruction; // Current Instruction
    unsigned char memory[4096]; // Memory

    // Registers
    unsigned char V[16]; // 16 General Purpose Registers (8-bits)
    unsigned short I;    // I Memory Address Register (16-bit)
    unsigned short pc;   // Program Counter (16-bit)

    // Stack
    unsigned short stack[16]; // Stack
    unsigned short sp; //Stack Pointer

    // Timers
    unsigned char delay_timer;
    unsigned char sound_timer;

    // Keypad
    unsigned char key[16];

    // Fontset
    unsigned char chip8_fontset[80] =
        {
            0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
            0x20, 0x60, 0x20, 0x20, 0x70, // 1
            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    void clearDisplay();

public:
    void initialize();
    bool loadGame(std::string fileName);
    void cycle();
    unsigned char readKeys();

    // Graphics
    unsigned char graphics[64 * 32];
    //Draw Flag
    bool drawFlag;
};
