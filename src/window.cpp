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
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);

    std::cout << "Window destroyed: " << window_name_ << std::endl;
}

void Window::initialise()
{
    // SDL_Init(SDL_INIT_EVERYTHING);

    window_ = SDL_CreateWindow(window_name_.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);

    renderer_ = SDL_CreateRenderer(window_, -1, 0);

    SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);

    SDL_RenderClear(renderer_);

    SDL_RenderPresent(renderer_);

    // SDL_Delay(3000);

    // SDL_DestroyRenderer(renderer);

    // SDL_DestroyWindow(window);

    // SDL_Quit();
}
} // namespace chip8emu