// The CHIP-8 instruction set is relatively simple, and includes
// mathematical and logical operations, control flow instructions,
// and instructions for handling input and output

// ADD - Add two registers together and store the result in a register.
// OR - Perform a bitwise OR operation on two registers and store the result in a register.
// XOR - Perform a bitwise XOR operation on two registers and store the result in a register.
// SHIFT - Shifts a register left or right by 1.

// Control flow instructions:
// JUMP - Unconditionally jump to a new memory location.
// CALL - Call a subroutine at a new memory location.
// RETURN - Return from a subroutine.
// SKIP - Skip the next instruction if a condition is true.

// Memory and register operations:
// LOAD - Load a value into a register.
// STORE - Store a value from a register into memory.
// COPY - Copy a value from one register to another.

// Input and output instructions:
// DRAW - Draw a sprite on the screen at a specified location.
// KEY - Check the state of a key and store the result in a register.
// Timer instructions:

// WAIT - Wait for a specified amount of time before continuing.
// SET - Set a timer to a specified value.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Arithmetic and logical operations:

uint8_t memory[4096];

const uint8_t chip8_fontset[80] =
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

void chip_startup(void)
{
    memcpy(memory + 80, chip8_fontset, sizeof(chip8_fontset));
}

int main(void)
{

    return 0;
}
