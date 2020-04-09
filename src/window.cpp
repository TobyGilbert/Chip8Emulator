#include "window.hpp"

#include <iostream>

namespace chip8emu
{
Window::Window(std::string window_name) : window_name_(window_name)
{
    initialise();
}

Window::~Window()
{
    destroy();
}

void Window::destroy()
{
    SDL_DestroyWindow(window_);

    std::cout << "Window destroyed: " << window_name_ << std::endl;
}

void Window::initialise()
{
    window_ = SDL_CreateWindow(window_name_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);

    if (!window_)
    {
        throw std::runtime_error("SDL failed to create window: " + window_name_);
    }

    surface_ = SDL_GetWindowSurface(window_);

    if (!surface_)
    {
        throw std::runtime_error("SDL failed to get window surface!");
    }

    SDL_UpdateWindowSurface(window_);
}
} // namespace chip8emu