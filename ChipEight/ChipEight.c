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

#define SCREEN_WIDTH  64
#define SCREEN_HEIGHT 32

#define FONTSET_START_ADDRESS 0x50

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
#define CHECK_X_RANGE(X) assert((X) < 16);
#define CHECK_Y_RANGE(Y) CHECK_X_RANGE(Y);

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

/*
Set all the elements of the gfx array to zero, effectively clearing the screen.
*/
INTERNAL INLINE void execute_00E0(void)
{
    memset(gfx, 0, sizeof(gfx));
}

/*
Jumps the program execution back to the address where the subroutine
was called, thus returning from the subroutine.
*/
INTERNAL INLINE void execute_00EE(void)
{
    SP--;
    PC = stack[SP];
}

/*
Does Nothing
*/
INTERNAL INLINE void execute_0NNN(void)
{
    // Do nothing
}

/*
address : The 12-bit address to set the program counter to

Sets the program counter (PC) to the first 12bits of the opcode,
effectively jumping to the specified address in memory.
Used for calling subroutines, once the subroutine is finished,
the program counter should point to the next instruction after the call.
*/
INTERNAL INLINE void execute_1NNN(const uint16_t opcode)
{
    const uint16_t address = opcode & 0x0FFFu;

    PC = address;
}

/*
address : The 12-bit address of the subroutine to call

The function first stores the current value of the program counter (PC)
on the stack, by assigning it to the current top of the stack and then
incrementing the stack pointer (SP). This allows the program to later
return to the instruction following the call.
*/
INTERNAL INLINE void execute_2NNN(const uint16_t opcode)
{
    const uint16_t address = opcode & 0x0FFFu;

    stack[SP++] = PC;
    PC          = address;
}

/*
x  : The index of the register VX
nn : The 8-bit immediate value to compare with the value of register VX

Skip the next instruction if the value of register VX is equal to NN
*/
INTERNAL INLINE void execute_3XNN(const uint8_t x, const uint8_t nn)
{
    CHECK_X_RANGE(x);

    if (V[x] == nn)
        PC += 2;
}

/*
x  : The index of the register VX
nn : The 8-bit immediate value to compare with the value of register VX

Skip the next instruction if the value of register VX is not equal to NN
*/
INTERNAL INLINE void execute_4XNN(const uint8_t x, const uint8_t nn)
{
    CHECK_X_RANGE(x);

    if (V[x] != nn)
        PC += 2;
}

/*
x : The index of the register VX
y : The index of the register VY

Skip the next instruction if the value of register VX is equal to the value of register VY
*/
INTERNAL INLINE void execute_5XY0(const uint8_t x, const uint8_t y)
{
    CHECK_XY_RANGE(x, y);

    if (V[x] == V[y])
        PC += 2;
}

/*
x  : The index of the register VX
nn : The 8-bit immediate value to be loaded into register VX

Sets the value of register VX to the immediate value NN passed as argument
*/
INTERNAL INLINE void execute_6XNN(const uint8_t x, const uint8_t nn)
{
    CHECK_X_RANGE(x);

    V[x] = nn;
}
void C8_execute_opcode(const uint16_t opcode)
{
    const uint8_t x  = (opcode & 0x0F00) >> 8;
    const uint8_t y  = (opcode & 0x00F0) >> 4;
    const uint8_t n  = (opcode & 0x000F);
    const uint8_t nn = (opcode & 0x000F);

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x00FF)
        {
        case 0x00E0:
            execute_00E0();
            break;
        case 0x00EE:
            execute_00EE();
            break;
        default:
                    execute_0NNN();
                    break;
            }
            break;
        case 0x1000:
            // const uint16_t nnn = (opcode & 0x0FFF);
            execute_1NNN(opcode);
            break;
        case 0x2000:
            // const uint16_t nnn = (opcode & 0x0FFF);
            execute_2NNN(opcode);
            break;
        case 0x3000:
            execute_3XNN(x, nn);
            break;
        case 0x4000:
            execute_4XNN(x, nn);
            break;
        case 0x5000:
            execute_5XY0(x, y);
            break;
        case 0x6000:
            execute_6XNN(x, nn);
            break;
            break;
        case 0x8000:
            switch (opcode & 0x000F)
            {
                default:
                    // handle unknown opcode
            break;
        }
        break;
    default:
        // handle unknown opcode
        break;
    }
}

}
