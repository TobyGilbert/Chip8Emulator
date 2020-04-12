#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Project headers
#include "emulator.hpp"

TEST(Emulator, CanBeInitialized)
{
    chip8emu::Emulator emulator;
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}