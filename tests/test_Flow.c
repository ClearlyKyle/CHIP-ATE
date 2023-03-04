#include "test_suite.h"

#include "ChipEight.c"

/*
Testing Opcodes:
00EE
1NNN
2NNN
BNNN
*/

void test_startup(void)
{
    C8_startup();
}

void test_teardown(void) {}

TEST_CASE(test_Function_execute_00EE)
{
    stack[0xA1] = 0x43;
    SP          = 0xA2;

    C8_execute_opcode(0x00EE);

    TEST_EXPECT_INT(PC, 0x43);
}

TEST_CASE(test_Function_execute_1NNN)
{
    const uint16_t nnn    = 0x123;
    const uint16_t opcode = 0x1123;

    PC = 0;

    C8_execute_opcode(opcode);

    TEST_EXPECT_INT(PC, nnn);
}

TEST_CASE(test_Function_execute_2NNN)
{
    const uint16_t nnn    = 0x123;
    const uint16_t opcode = 0x2123;

    SP = 0xA1;
    PC = 0xB0;

    C8_execute_opcode(opcode);

    TEST_EXPECT_INT(SP, 0xA2);
    TEST_EXPECT_INT(stack[0xA1], 0xB0 + 2); // +2 because the PC +=2 on execute
    TEST_EXPECT_INT(PC, nnn);
}

TEST_CASE(test_Function_execute_BNNN)
{
    const uint16_t nnn    = 0x321;
    const uint16_t opcode = 0xB321;

    V[0] = 0xA1;

    C8_execute_opcode(opcode);

    TEST_EXPECT_INT(PC, 0xA1 + nnn);
}

int main()
{
    TEST_RUN(test_Function_execute_00EE);
    TEST_RUN(test_Function_execute_1NNN);
    TEST_RUN(test_Function_execute_2NNN);
    TEST_RUN(test_Function_execute_BNNN);

    TEST_SUMMARY();

    return 0;
}