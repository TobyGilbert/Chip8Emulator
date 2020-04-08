#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <SDL.h>
#include <string>

namespace chip8emu
{
class Window
{
public:
    Window(std::string window_name = "Default");
    ~Window();
    void initialise();

private:
    void destroy();

    SDL_Window *window_;
    SDL_Renderer *renderer_;

    std::string window_name_;
};
} // namespace chip8emu

#endif // WINDOW_HPP_