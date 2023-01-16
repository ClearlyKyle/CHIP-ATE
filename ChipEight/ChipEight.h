/*
    00E0, // Clear the screen
    00EE, // Return from subroutine
    0NNN, // Does Nothing
    1NNN, // Jump to location NNN
    2NNN, // Call subroutine at NNN
    3XNN, // Skip next instruction if vx == NN.
    4XNN, // Skip next instruction if vx != NN.
    5XY0, // Skip next instruction if vx == vy.
    6XNN, // vx := NN
    7XNN, // vx += NN
    8XY0, // vx := vy
    8XY1, // vx |= vy Bitwise OR
    8XY2, // vx &= vy Bitwise AND
    8XY3, // vx ^= vy Bitwise XOR
    8XY4, // vx += vy vf = 1 on carry
    8XY5, // vx -= vy vf = 0 on borrow
    8XY6, // vx >>= vy vf = old least significant bit
    8XY7, // vx =- vy vf = 0 on borrow
    8XYE, // vx <<= vy vf = old most significant bit
    9XY0, // if vx == vy then
    ANNN, // i := NNN
    BNNN, // jump0 NNN Jump to address NNN + v0
    CXNN, // vx := random NN Random number 0-255 AND NN
    DXYN, // sprite vx vy N vf = 1 on collision
    EX9E, // if vx -key then Is a key not pressed?
    EXA1, // if vx key then Is a key pressed?
    FX07, // vx := delay
    FX0A, // vx := key Wait for a keypress
    FX15, // delay := vx
    FX18, // buzzer := vx
    FX1E, // i += vx
    FX29, // i := hex vx Set i to a hex character
    FX33, // bcd vx Decode vx into binary-coded decimal
    FX55, // save vx Save v0-vx to i through (i+x)
    FX65, // load vx Load v0-vx from i through (i+x)
*/
#ifndef __CHIPEIGHT_H__
#define __CHIPEIGHT_H__

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

void C8_startup(void);
void C8_load_memory(const uint8_t *program_memory, const size_t size);
void C8_execute_opcode(const uint16_t opcode);

#endif // __CHIPEIGHT_H__