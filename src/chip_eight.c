#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG_MODE
#define DEBUG_LEVEL 0

#define LOW_RES_WIDTH   (64)
#define LOW_RES_HEIGHT  (32)
#define HIGH_RES_WIDTH  (128)
#define HIGH_RES_HEIGHT (64)

#define bool  uint8_t
#define false (0)
#define true  (1)

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

#define LOG_ERROR(msg, ...) \
    fprintf(DEBUG_STREAM, "(" __FILE__ ":%d) " msg, __LINE__, ##__VA_ARGS__), __debugbreak()

#ifdef DEBUG_MODE
    #define DEBUG_STREAM stderr
    #if DEBUG_LEVEL > 2
        #define LOG_VERBOSE(msg, ...) \
            fprintf(DEBUG_STREAM, "(" __FILE__ ":%d) " msg, __LINE__, ##__VA_ARGS__)
    #else
        #define LOG_VERBOSE(msg, ...)
    #endif

    #if DEBUG_LEVEL > 1
        #define LOG_INFO(msg, ...) \
            fprintf(DEBUG_STREAM, "(" __FILE__ ":%d) " msg, __LINE__, ##__VA_ARGS__)
    #else
        #define LOG_INFO(msg, ...)
    #endif
#else
    #define LOG_VERBOSE(msg, ...)
    #define LOG_INFO(msg, ...)
    #define SDL_CHECK(fn) fn
#endif

enum emulator_state
{
    QUIT,
    PAUSED,
    RUNNING
};
struct chip_eight
{
    // TODO : can we combine this with "draw graphics bits"?
    enum emulator_state state;
    enum extension      exten;

    bool draw;

    uint8_t ram[4096];
    uint8_t display[HIGH_RES_WIDTH * HIGH_RES_HEIGHT];
    uint8_t disp_w, disp_h;

    uint16_t stack[12]; // subroutine stack
    uint8_t  stack_pos;
    uint8_t  V[16];       // data registers V0-VF
    uint16_t I;           // index register
    uint16_t PC;          // program counter
    uint8_t  delay_timer; // decrements at 60hz when >0
    uint8_t  sound_timer; // decrements at 60hz and plays a tone when >0

    uint16_t keypad; // hexadecimal keybad 0x0 -> 0xF
};

static void chip_eight_init(struct chip_eight *c8, const char *rom_name, enum extension exten, uint8_t w, uint8_t h)
{
    if (c8 == NULL)
    {
        LOG_ERROR("Pointer to c8 cannot be NULL");
        return;
    }

    const uint16_t entry_point = 0x200; // chip8 roms will be loaded to this address
    const uint8_t  font[]      = {
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
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };                                // TODO : would this be better as a 2D array?

    // load font
    memcpy(&c8->ram[0], font, sizeof(font));

    // load rom
    FILE   *fp  = NULL;
    errno_t err = fopen_s(&fp, rom_name, "rb");
    if (err != 0 || !fp)
    {
        LOG_ERROR("Unable to open rom : %s", rom_name);
        return;
    }

    // TODO : Better file handling here...
    // file size
    fseek(fp, 0, SEEK_END);
    const long rom_size = ftell(fp);
    const long max_size = sizeof(c8->ram) - entry_point;
    rewind(fp);

    if (rom_size > max_size)
    {
        LOG_ERROR("Rom too big for RAM : %s\n Max size: %d, Rom size: %d", rom_name, max_size, rom_size);
        err = fclose(fp);
        if (err != 0)
        {
            LOG_ERROR("Unable to close rom : %s", rom_name);
            return;
        }
        return;
    }

    if (fread(&c8->ram[entry_point], rom_size, 1, fp) != 1)
    {
        LOG_ERROR("Unable to read the data from the rom to the ram");

        err = fclose(fp);
        if (err != 0)
        {
            LOG_ERROR("Unable to close rom : %s", rom_name);
            return;
        }
    }

    err = fclose(fp);
    if (err != 0)
    {
        LOG_ERROR("Unable to close rom : %s", rom_name);
        return;
    }

    LOG_INFO("'%s', rom was loaded successfuly", rom_name);
    LOG_INFO("rom size : %dbytes", rom_size);

    srand((unsigned int)time(NULL));

    // set chip8 defaults
    c8->state = RUNNING;
    c8->exten = exten;
    c8->PC    = entry_point;
    c8->draw  = false;

    memset(&c8->display[0], 0, sizeof(c8->display));
    c8->disp_w = w;
    c8->disp_h = h;
}

inline void chip_eight_reset(struct chip_eight *c8)
{
    c8->state = RUNNING;
    // uint8_t  ram[4096];
    memset(&c8->display[0], 0, sizeof(c8->display));
    memset(&c8->stack[0], 0, sizeof(c8->stack));
    c8->stack_pos = 0;
    memset(&c8->V[0], 0, sizeof(c8->V));

    c8->I  = 0;
    c8->PC = 0x200;

    c8->delay_timer = 0;
    c8->sound_timer = 0;
    c8->draw        = false;

    c8->keypad = 0;
}

inline void chip_eight_clear_screen(struct chip_eight *const c8)
{
    memset(&c8->display[0], 0, sizeof(c8->display));
}

void chip_eight_emulate(struct chip_eight *const c8)
{
    if (c8 == NULL)
    {
        LOG_ERROR("Pointer to c8 cannot be NULL");
        return;
    }

    const uint16_t opcode = ((c8->ram[c8->PC]) << 8) | c8->ram[c8->PC + 1];
    c8->PC += 2; // increment PC for next opcode

    const uint16_t NNN = opcode & 0x0FFF;
    const uint8_t  NN  = opcode & 0x00FF;
    const uint8_t  N   = opcode & 0x000F;

    // DXYN
    const uint8_t X = (opcode >> 8) & 0x000F;
    const uint8_t Y = (opcode >> 4) & 0x000F;

    LOG_VERBOSE("[0x%4X] ", c8->PC - 2);

    switch ((opcode >> 12) & 0x000F)
    {
    default:
        LOG_ERROR("Unimplemented opcode: 0x%04X", opcode);
    }
}
