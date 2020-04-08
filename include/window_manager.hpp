#ifndef WINDOW_MANAGER_HPP_
#define WINDOW_MANAGER_HPP_

#include <string>
#include <map>

#include "window.hpp"

namespace chip8emu
{
class WindowManager
{
public:
    WindowManager();
    ~WindowManager();

    void AddWindow(std::string window_name);

private:
    void Initialise();
    void Destroy();

    std::map<std::string, std::unique_ptr<Window>> windows_;
};
} // namespace chip8emu

#endif // WINDOW_MANAGER_HPP_