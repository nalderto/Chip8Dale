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
        break;
    case 0x1000: // 1NNN: Jump to address NNN
        pc = instruction & 0x0FFF;
        break;
    case 0x2000: // 2NNN: Calls subroutine at NNN
        stack[sp] = pc;
        sp++;
        pc = instruction & 0x0FFF;
        break;
    case 0x3000: // 3XNN: Skips the next instruction if VX equals NN
        if (V[instruction & 0x0F00 >> 8] == (instruction & 0x00FF))
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;
    case 0x4000: // 4XNN: Skips the next instruction if VX not equal NN
        if (V[instruction & 0x0F00 >> 8] != (instruction & 0x00FF))
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;
    case 0x5000: // 5XNN: Skips the next instruction if VX equals VY
        if (V[(instruction & 0x0F00) >> 8] == V[instruction & 0x00F0 >> 4])
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
        break;
    case 0x6000: // 6XNN: Sets VX to NN
        V[(instruction & 0x0F00) >> 8] = (instruction & 0x00FF);
        break;
    case 0x7000: // 7XNN: Adds NN to VX (Carry flag is not changed)
        V[(instruction & 0x0F00) >> 8] += (instruction & 0x00FF);
        break;
    case 0x8000:
        switch (instruction & 0x000F)
        {
        case 0x0000: // 8XY0: Sets VX to the value of VY
            V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x00f0) >> 4];
            break;
        case 0x0001: // 8XY1: Sets VX to VX OR VY.
            V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x0F00) >> 8] | V[(instruction & 0x00f0) >> 4];
            break;
        case 0x0002: //8XY2: Sets VX to VX AND VY
            V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x0F00) >> 8] & V[(instruction & 0x00f0) >> 4];
            break;
        case 0x0003: //8XY3: Sets VX to VX XOR VY
            V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x0F00) >> 8] ^ V[(instruction & 0x00f0) >> 4];
            break;
        case 0x0004: //8XY4: Adds VY to VX
            if (V[(instruction & 0x0F00) >> 8] + V[(instruction & 0x00f0) >> 4] > 255)
            {
                V[15] = 1;
            }
            else
            {
                V[15] = 0;
            }
            V[(instruction & 0x0F00) >> 8] += V[(instruction & 0x00f0) >> 4];
            break;
        case 0x0005: //8XY5: VY is subtracted from VX
            if (V[(instruction & 0x0F00) >> 8] > V[(instruction & 0x00f0) >> 4])
            {
                V[15] = 1;
            }
            else
            {
                V[15] = 0;
            }
            V[(instruction & 0x0F00) >> 8] -= V[(instruction & 0x00f0) >> 4];
            break;
        case 0x0006: //8XY6: Stores the least significant bit of VX in VF and then shifts VX to the right by 1
            V[16] = V[(instruction & 0x0F00) >> 8] & 1;
            V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x0F00) >> 8] >> 1;
            break;
        case 0x0007: //8XY7: Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't
            if (V[(instruction & 0x0F00) >> 4] > V[(instruction & 0x00f0) >> 8])
            {
                V[15] = 1;
            }
            else
            {
                V[15] = 0;
            }
            V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x00f0) >> 4] - V[(instruction & 0x0F00) >> 8];
            break;
        case 0x000E: //8XYE: Stores the most significant bit of VX in VF and then shifts VX to the left by 1
            V[16] = V[(instruction & 0x0F00) >> 8] & 0x80;
            V[(instruction & 0x0F00) >> 8] = V[(instruction & 0x0F00) >> 8] << 1;
            break;
        default:
            printf("Unknown instruction: 0x%X\n", instruction);
            break;
        }
        pc += 2;
        break;
    case 0x9000: // 9XY0: Skip next instruction if VX != VY
        if (V[instruction & 0x0F00 >> 8] != V[instruction & 0x0F00 >> 4])
        {
            pc += 4;
        }
        else
        {
            pc += 2;
        }
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
    case 0xD000: // DXYN: Draws sprite at coordinate VX, VY
        break;
    case 0xE000:
        switch (instruction & 0x00FF)
        {
        case 0x009E: // EX9E: Skips the next instruction if the key stored in VX isn't pressed
            if (key[V[(instruction & 0x0F00) >> 8]] != 0)
                pc += 4;
            else
                pc += 2;
            break;
        case 0x00A1: // EXA1: Skips the next instruction if the key stored in VX isn't pressed
            if (key[V[(instruction & 0x0F00) >> 8]] == 0)
                pc += 4;
            else
                pc += 2;
            break;
        default:
            printf("Unknown instruction: 0x%X\n", instruction);
            break;
        }
        break;
    case 0xF000:
        switch (instruction & 0x00FF)
        {
        case 0x0007: // FX07: Sets VX to the value of the delay timer
            V[(instruction & 0x0F00) >> 8] == delay_timer;
            break;
        case 0x000A: // FX0A: A key press is awaited, and then stored in VX
            break;
        case 0x0015: // FX15: Sets the delay timer to VX
            delay_timer = V[(instruction & 0x0F00) >> 8];
            break;
        case 0x0018: // FX18: Sets the sound timer to VX
            sound_timer = V[(instruction & 0x0F00) >> 8];
            break;
        case 0x001E: //FX1E: Adds VX to I
            I += V[(instruction & 0x0F00) >> 8];
            break;
        case 0x0029:
            /* code */
            break;
        case 0x0033:
            /* code */
            break;
        case 0x0055:
            /* code */
            break;
        case 0x0065:
            /* code */
            break;

        default:
            printf("Unknown instruction: 0x%X\n", instruction);
            break;
        }
        pc += 2;
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