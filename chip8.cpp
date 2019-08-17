#include "chip8.hpp"

void chip8::initialize()
{
    pc = 0x200;      // Program Counter Starts at 0x200
    instruction = 0; // Reset Current Instruction
    I = 0;           // Reset I Register
    sp = 0;          // Reset Stack Pointer
    delay_timer = 0; // Reset Delay Timer
    sound_timer = 0; // Reset Sound Timer

    // Reset General Purpose Registers & Stack
    for (int i = 0; i < 16; i++)
    {
        V[i] = 0;     // General Purpose Registers
        stack[i] = 0; // Stack
    }

    // Reset Graphics
    for (int i = 0; i < (64 * 32); i++)
    {
        graphics[i] = 0;
    }

    // Reset Graphics
    clearDisplay();

    // Reset Memory
    for (int i = 0; i < 4096; i++)
    {
        memory[i] = 0;
    }

    // Load Fontset
    for (int i = 0; i < 80; i++)
    {
        memory[i] = chip8_fontset[i];
    }
};

bool chip8::loadGame(std::string fileName)
{
    FILE *fp;
    try
    {
        fp = fopen(fileName.c_str(), "rb");
        int i = 0;
        while (!feof(fp))
        {
            fread(&memory[512 + i], sizeof(unsigned char), 1, fp);
            i++;
        }
        return true;
    }
    catch (int e)
    {
        return false;
    }
};

void chip8::cycle()
{
    instruction = memory[pc] << 8 | memory[pc + 1];

    switch (instruction & 0xF000)
    {
    case 0x0000: 
        switch (instruction & 0x000F)
        {
        case 0x0000: // 0x00E0: Clears the screen
            clearDisplay();
            pc += 2;
            break;
        case 0x000E: // 0x00EE: Returns from a subroutine
            sp--;
            pc = stack[sp];
            break;
        
        default:
            printf("Unknown instruction: 0x%X\n", instruction);
            break;
        }
    case 0x1000: // 1NNN: Jump to address NNN
        pc = instruction & 0x0FFF;
        break;
    case 0x2000: // 2NNN: Calls subroutine at NNN
        stack[sp] = pc;
        sp++;
        pc = instruction & 0x0FFF;
        break;
    case 0x3000: // 3XNN: Skips the next instruction if VX equals NN
        if (V[instruction & 0x0F00 >> 8] == (instruction & 0x00FF)) {
            pc += 4;
        }
        else {
            pc += 2;
        }
        break;
    case 0x4000: // 4XNN: Skips the next instruction if VX not equal NN
        if (V[instruction & 0x0F00 >> 8] != (instruction & 0x00FF)) {
            pc += 4;
        }
        else {
            pc += 2;
        }
        break;
    case 0x5000: // 5XNN: Skips the next instruction if VX equals VY
        if (V[(instruction & 0x0F00) >> 8] == V[instruction & 0x00F0 >> 4]) {
            pc += 4;
        }
        else {
            pc += 2;
        }
        break;
    case 0x6000: // 6XNN: Sets VX to NN
        V[(instruction & 0x0F00) >> 8] = (instruction & 0x00FF);
        break;
    case 0x7000: // 7XNN: Adds NN to VX (Carry flag is not changed)
        V[(instruction & 0x0F00) >> 8] += (instruction & 0x00FF);
        break;
    case 0xA000: // ANNN: Sets I to the address NNN
        I = instruction & 0x0FFF;
        pc += 2;
        break;
    case 0xB000: // BNNN: Jump to the address NNN plus V0
        pc = V[0] + (instruction & 0x0FFF);
        break;
    case 0xC000: // CXNN: Sets VX = bitwise AND operation on a random number and NN
        V[instruction & 0x0F00] = (rand() % 256) & (instruction & 0x00FF);
        pc += 2;
        break;

    default:
        printf("Unknown instruction: 0x%X\n", instruction);
        break;
    }

    if (delay_timer > 0)
    {
        delay_timer--;
    }

    if (sound_timer == 1)
    {
        std::cout << "BEEP!" << std::endl;
        sound_timer--;
    }
}

void chip8::clearDisplay()
{
    for (int i = 0; i < (64 * 32); i++)
    {
        graphics[i] = 0;
    }
}