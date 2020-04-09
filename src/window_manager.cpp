#include "window_manager.hpp"

#include <SDL.h>
#include <exception>
#include <iostream>

namespace chip8emu
{
WindowManager::WindowManager()
{
    Initialise();
}

WindowManager::~WindowManager()
{
    Destroy();
}

void WindowManager::Initialise()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw std::runtime_error("SDL failed to initialise!");
    }
}

void WindowManager::Destroy()
{
    // Delete the windows here before terminating SDL
    for (std::map<std::string, std::unique_ptr<Window>>::value_type &window : windows_)
    {
        window.second.reset();
    }

    SDL_Quit();

    std::cout << "Destroy WindowManager" << std::endl;
}

void WindowManager::AddWindow(std::string window_name)
{
    if (windows_.find(window_name) != windows_.end())
    {
        throw std::invalid_argument("WindowManager already contains a window with name: " + window_name);
    }

    windows_.insert({std::string(window_name), std::make_unique<Window>(window_name)});
}
} // namespace chip8emu