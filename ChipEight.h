#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Data registers
uint8_t V[16];

// Special purpose registers
uint16_t PC;
uint8_t SP;

// Memory
uint8_t memory[4096];

// Flags
uint8_t carry_flag;
uint8_t borrow_flag;

// Video memory
uint8_t gfx[64 * 32];

// Screen needs to be updated or not
uint8_t draw_flag = 0;

enum Opcode
{
    ADD,
    OR,
    XOR,
    SHIFT,
    // other opcodes
};

enum Opcode
{
    OPCODE_00E0, // clear
    OPCODE_00EE, // return Exit a subroutine
    OPCODE_1NNN, // jump NNN
    OPCODE_2NNN, // NNN Call a subroutine
    OPCODE_3XNN, // if vx != NN then
    OPCODE_4XNN, // if vx == NN then
    OPCODE_5XY0, // if vx != vy then
    OPCODE_6XNN, // vx := NN
    OPCODE_7XNN, // vx += NN
    OPCODE_8XY0, // vx := vy
    OPCODE_8XY1, // vx |= vy Bitwise OR
    OPCODE_8XY2, // vx &= vy Bitwise AND
    OPCODE_8XY3, // vx ^= vy Bitwise XOR
    OPCODE_8XY4, // vx += vy vf = 1 on carry
    OPCODE_8XY5, // vx -= vy vf = 0 on borrow
    OPCODE_8XY6, // vx >>= vy vf = old least significant bit
    OPCODE_8XY7, // vx =- vy vf = 0 on borrow
    OPCODE_8XYE, // vx <<= vy vf = old most significant bit
    OPCODE_9XY0, // if vx == vy then
    OPCODE_ANNN, // i := NNN
    OPCODE_BNNN, // jump0 NNN Jump to address NNN + v0
    OPCODE_CXNN, // vx := random NN Random number 0-255 AND NN
    OPCODE_DXYN, // sprite vx vy N vf = 1 on collision
    OPCODE_EX9E, // if vx -key then Is a key not pressed?
    OPCODE_EXA1, // if vx key then Is a key pressed?
    OPCODE_FX07, // vx := delay
    OPCODE_FX0A, // vx := key Wait for a keypress
    OPCODE_FX15, // delay := vx
    OPCODE_FX18, // buzzer := vx
    OPCODE_FX1E, // i += vx
    OPCODE_FX29, // i := hex vx Set i to a hex character
    OPCODE_FX33, // bcd vx Decode vx into binary-coded decimal
    OPCODE_FX55, // save vx Save v0-vx to i through (i+x)
    OPCODE_FX65, // load vx Load v0-vx from i through (i+x)
};

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
