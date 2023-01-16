#include "ChipEight.h"

#if defined(_MSC_VER)
#define INLINE __inline
#else
#define INLINE inline
#endif

#define INTERNAL static

INTERNAL const uint8_t chip8_fontset[80] =
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

// Flags
INTERNAL uint8_t carry_flag  = 0;
INTERNAL uint8_t borrow_flag = 0;
INTERNAL uint8_t draw_flag   = 0; // Screen needs to be updated or not

INTERNAL uint8_t  V[16]        = {0}; // Data registers
INTERNAL uint8_t  memory[4096] = {0}; // Memory
INTERNAL uint8_t  gfx[64 * 32] = {0}; // Video memory, value 0 pixel is off, value 1 is on
INTERNAL uint16_t stack[16]    = {0}; // Program Stack

INTERNAL uint16_t PC = 0; // Program Counter
INTERNAL uint8_t  SP = 0; // Stack Pointer

// takes a 16-bit VALUE and stores it on the top of the stack
// stack pointer is incremented before assigment
#define STACK_PUSH(VALUE) stack[SP++] = (VALUE)

// returns the value on the top of the stack
// and decrements the stack pointer
#define STACK_POP(VALUE) stack[--SP]

// perforn a check for outof bounds register index
#define CHECK_XY_RANGE(X, Y) assert(x < 16 && y < 16);
#define CHECK_X_RANGE(X)     assert((X));
#define CHECK_Y_RANGE(Y)     CHECK_X_RANGE(Y);

// initialises memory to 0 and loads the fontset into memory
void C8_startup(void)
{
    memset(memory, 0, 4096);
    memcpy_s(&memory[0x50], sizeof(memory), chip8_fontset, sizeof(uint8_t) * 80);
}

void C8_load_memory(const uint8_t *program_memory, const size_t size)
{
    if (program_memory == NULL && size <= 2)
        return;

    uint8_t *start_address = &memory[0x200];

    memcpy_s(start_address, 4096 - 0x200, (const void *)program_memory, size);
}

void C8_execute_opcode(const uint16_t opcode)
{
}

}
