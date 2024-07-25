#include "test_suite.h"

#include "ChipEight.c"

/*
Testing Opcodes:
8XY1
8XY2
8XY3
8XY6
8XYE
*/

void test_startup(void)
{
    C8_startup();
}

void test_teardown(void) {}

// OR
TEST_CASE(test_Function_execute_8XY1)
{
    // X and Y indicies
    const uint8_t X = 0;
    const uint8_t Y = 1;

    // Test 1: Check result when VX and VY both contain 1's
    V[X]                    = 0x01;
    V[Y]                    = 0x01;
    uint8_t expected_result = 0x01;

    C8_execute_opcode(0x8011);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 2: Check result when VX contains 1's and VY contains 0's
    V[X]            = 0x01;
    V[Y]            = 0x00;
    expected_result = 0x01;

    C8_execute_opcode(0x8011);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 3: Check result when VX contains 0's and VY contains 1's
    V[X]            = 0x00;
    V[Y]            = 0x01;
    expected_result = 0x01;

    C8_execute_opcode(0x8011);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 4: Check result when VX and VY both contain 0's
    V[X]            = 0x00;
    V[Y]            = 0x00;
    expected_result = 0x00;

    C8_execute_opcode(0x8011);
    TEST_EXPECT_INT(V[X], expected_result);
}

// AND
TEST_CASE(test_Function_execute_8XY2)
{
    // X and Y indicies
    const uint8_t X = 0;
    const uint8_t Y = 1;

    // Test 1: Check result when VX and VY both contain 1's
    V[X]                    = 0x01;
    V[Y]                    = 0x01;
    uint8_t expected_result = 0x01;

    C8_execute_opcode(0x8012);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 2: Check result when VX contains 1's and VY contains 0's
    V[X]            = 0x01;
    V[Y]            = 0x00;
    expected_result = 0x00;

    C8_execute_opcode(0x8012);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 3: Check result when VX contains 0's and VY contains 1's
    V[X]            = 0x00;
    V[Y]            = 0x01;
    expected_result = 0x00;

    C8_execute_opcode(0x8012);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 4: Check result when VX and VY both contain 0's
    V[X]            = 0x00;
    V[Y]            = 0x00;
    expected_result = 0x00;

    C8_execute_opcode(0x8012);
    TEST_EXPECT_INT(V[X], expected_result);
}

// XOR
TEST_CASE(test_Function_execute_8XY3)
{
    // X and Y indicies
    const uint8_t X = 0;
    const uint8_t Y = 1;

    // Test 1: Check result when VX and VY both contain 1's
    V[X]                    = 0x01;
    V[Y]                    = 0x01;
    uint8_t expected_result = 0x00;

    C8_execute_opcode(0x8013);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 2: Check result when VX contains 1's and VY contains 0's
    V[X]            = 0x01;
    V[Y]            = 0x00;
    expected_result = 0x01;

    C8_execute_opcode(0x8013);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 3: Check result when VX contains 0's and VY contains 1's
    V[X]            = 0x00;
    V[Y]            = 0x01;
    expected_result = 0x01;

    C8_execute_opcode(0x8013);
    TEST_EXPECT_INT(V[X], expected_result);

    // Test 4: Check result when VX and VY both contain 0's
    V[X]            = 0x00;
    V[Y]            = 0x00;
    expected_result = 0x00;

    C8_execute_opcode(0x8013);
    TEST_EXPECT_INT(V[X], expected_result);
}

// Right Shift
TEST_CASE(test_Function_execute_8XY6)
{
    // VX register to 0xAB
    V[0xA] = 0xAB;

    // Execute opcode 0x8A06, which should shift Vx right by 1
    C8_execute_opcode(0x8A06);

    // Verify that Vx now contains the expected value (0x55)
    TEST_EXPECT_INT(V[0xA], 0x55);
}

// Left Shift
TEST_CASE(test_Function_execute_8XYE_MSB_is_set)
{
    // Set opcode 0x810E, which should shift V1 left and store the most significant bit in VF
    const uint16_t opcode = 0x810E;

    // Initialize register VX
    V[0x1] = 0b11001100;

    // Execute the opcode
    C8_execute_opcode(opcode);

    // Verify that V1 has been shifted left by one
    TEST_EXPECT_INT(V[0x1], 0b10011000);

    // Verify that VF is set to 1, since the most significant bit of V1 was 1
    TEST_EXPECT_INT(V[0xF], 1);
}

TEST_CASE(test_Function_execute_8XYE_MSB_is_NOT_set)
{
    // Set opcode 0x810E, which should shift V1 left and store the most significant bit in VF
    const uint16_t opcode = 0x810E;

    // Initialize register VX
    V[0x1] = 0b01001100;

    // Execute the opcode
    C8_execute_opcode(opcode);

    // Verify that V1 has been shifted left by one
    TEST_EXPECT_INT(V[0x1], 0b10011000);

    // Verify that VF is set to 1, since the most significant bit of V1 was 1
    TEST_EXPECT_INT(V[0xF], 0);
}

int main()
{
    TEST_RUN(test_Function_execute_8XY1);
    TEST_RUN(test_Function_execute_8XY2);
    TEST_RUN(test_Function_execute_8XY3);
    TEST_RUN(test_Function_execute_8XY6);

    TEST_RUN(test_Function_execute_8XYE_MSB_is_set);
    TEST_RUN(test_Function_execute_8XYE_MSB_is_NOT_set);

    TEST_SUMMARY();

    return 0;
}