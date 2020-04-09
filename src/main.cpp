#include <iostream>
#include "emulator.hpp"
#include "window_manager.hpp"

const std::string rom_file_path = "roms/TETRIS.c8";

int main()
{
    try
    {
        // Setup render system and register input callbacks
        chip8emu::WindowManager window_manager;
        window_manager.AddWindow("Emulator");

        // Initialise the Chip8 system and load the game into memory
        chip8emu::Emulator emulator;

        emulator.LoadROM(rom_file_path);

        // Emulation loop
        SDL_Event e;
        bool quit = false;
        while (!quit)
        {
            // Emulate one cycle
            emulator.EmulateCycle();

            // If the draw flag is set, update the screen

            // Store the key press state
            while (SDL_PollEvent(&e))
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN)
                {
                    quit = true;
                }
            }
        }
    }
    catch (std::exception &exception)
    {
        std::cout << exception.what() << std::endl;
    }

    return 0;
}