#ifndef WINDOW_MANAGER_HPP_
#define WINDOW_MANAGER_HPP_

#include <string>
#include <map>
#include <memory>

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
    WindowManager(WindowManager const &other);
    WindowManager &operator=(WindowManager const &other);

    void Initialise();
    void Destroy();

    std::map<std::string, std::unique_ptr<Window>> windows_;
    // std::unique_ptr<Window> window_;
};
} // namespace chip8emu

#endif // WINDOW_MANAGER_HPP_