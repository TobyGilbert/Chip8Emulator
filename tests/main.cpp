#include "gtest/gtest.h"
#include "gmock/gmock.h"

// Project headers
#include "emulator.hpp"
#include "window.hpp"
#include "window_manager.hpp"

TEST(Emulator, CanBeInitialized)
{
    chip8emu::Emulator emulator;
}

TEST(Window, CanBeInitialised)
{
    auto window_manager = chip8emu::WindowManager();

    window_manager.AddWindow("test");
}

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}