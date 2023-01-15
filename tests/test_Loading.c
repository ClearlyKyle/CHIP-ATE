#include "test_suite.h"

#include "ChipEight.c"

void test_startup(void)
{
    chip_startup();
}

void test_teardown(void)
{
}

TEST_CASE(test_Can_Load_A_Program_Into_Memory)
{
    uint8_t test_memory[4096] = {0};

    test_memory[1] = 10;
    test_memory[2] = 11;
    test_memory[3] = 12;
    test_memory[4] = 13;
    test_memory[5] = 14;
    test_memory[6] = 15;

    load_memory(test_memory, 7);

    TEST_ASSERT(memory[0x200 + 1] == 10);
    TEST_ASSERT(memory[0x200 + 2] == 11);
    TEST_ASSERT(memory[0x200 + 3] == 12);
    TEST_ASSERT(memory[0x200 + 4] == 13);
    TEST_ASSERT(memory[0x200 + 5] == 14);
    TEST_ASSERT(memory[0x200 + 6] == 15);
}

TEST_CASE(test_Font_Set_Is_Loaded_Correctly)
{
    // chip8_fontset should be inserted starting at
    // address 0x50 (index 80)
    uint8_t *starting_location = &memory[0x50];
    for (uint8_t i = 0; i < 80; i++) // 80 - index count of chip8_fontset
    {
        TEST_EXPECT_INT(*(starting_location + i), chip8_fontset[i]);
    }
}

int main()
{
    TEST_RUN(test_Can_Load_A_Program_Into_Memory);
    TEST_RUN(test_Font_Set_Is_Loaded_Correctly);

    TEST_SUMMARY();

    return 0;
}