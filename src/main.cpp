#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "emulator.hpp"
#include "renderer.hpp"

const std::string rom_file_path = "roms/tetris.c8";

struct ApplicationData
{
    chip8emu::Emulator *emulator;
    chip8emu::Renderer *renderer;
};

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    auto data = reinterpret_cast<ApplicationData *>(glfwGetWindowUserPointer(window));
    auto emulator = data->emulator;
    // Key Pressed
    {
        if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_1);
        }
        if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_2);
        }
        if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_3);
        }
        if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_4);
        }
        if (key == GLFW_KEY_Q && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_Q);
        }
        if (key == GLFW_KEY_W && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_W);
        }
        if (key == GLFW_KEY_E && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_E);
        }
        if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_R);
        }
        if (key == GLFW_KEY_A && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_A);
        }
        if (key == GLFW_KEY_S && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_S);
        }
        if (key == GLFW_KEY_D && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_D);
        }
        if (key == GLFW_KEY_F && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_F);
        }
        if (key == GLFW_KEY_Z && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_Z);
        }
        if (key == GLFW_KEY_X && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_X);
        }
        if (key == GLFW_KEY_C && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_C);
        }
        if (key == GLFW_KEY_V && action == GLFW_PRESS)
        {
            emulator->KeyPressed(emulator->KEYCODE_V);
        }
    }

    // Key Releases
    {
        if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_1);
        }
        if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_2);
        }
        if (key == GLFW_KEY_3 && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_3);
        }
        if (key == GLFW_KEY_4 && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_4);
        }
        if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_Q);
        }
        if (key == GLFW_KEY_W && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_W);
        }
        if (key == GLFW_KEY_E && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_E);
        }
        if (key == GLFW_KEY_R && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_R);
        }
        if (key == GLFW_KEY_A && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_A);
        }
        if (key == GLFW_KEY_S && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_S);
        }
        if (key == GLFW_KEY_D && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_D);
        }
        if (key == GLFW_KEY_F && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_F);
        }
        if (key == GLFW_KEY_Z && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_Z);
        }
        if (key == GLFW_KEY_X && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_X);
        }
        if (key == GLFW_KEY_C && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_C);
        }
        if (key == GLFW_KEY_V && action == GLFW_RELEASE)
        {
            emulator->KeyReleased(emulator->KEYCODE_V);
        }
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // renderer.Resize(width, height);
}

int main()
{
    try
    {
        if (!glfwInit())
        {
            throw std::runtime_error("Failed to initialise GLFW!");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        auto window = glfwCreateWindow(64 * 10, 32 * 10, "Emulator", NULL, NULL);

        if (!window)
        {
            glfwTerminate();

            throw std::runtime_error("Failed to initialise window");
        }

        // GLFW callbacks
        glfwSetKeyCallback(window, key_callback);
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            glfwDestroyWindow(window);
            glfwTerminate();
            throw std::runtime_error("Failed to initialise GLAD");
        }

        chip8emu::Renderer renderer;
        renderer.Resize(64 * 10, 32 * 10);

        // Initialise the Chip8 system and load the game into memory
        chip8emu::Emulator emulator;

        ApplicationData data{&emulator, &renderer};
        glfwSetWindowUserPointer(window, &data);

        emulator.LoadROM(rom_file_path);

        // Emulation loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();

            // Emulate one cycle
            emulator.EmulateCycle();

            // If the draw flag is set, update the screen
            if (emulator.DoesScreenNeedRedraw())
            {
                unsigned char data[64 * 32];
                auto pixels = emulator.GetScreenPixels();

                unsigned int data_index = 0;
                unsigned int x_index = 0;
                // Flip rows
                for (auto y = 32 - 1; y >= 0; y--)
                {
                    for (auto x = 0; x < 64; x++)
                    {
                        data[data_index++] = pixels[x + (y * 64)] * 255;
                    }
                }
                renderer.UpdateTexture(data);
                emulator.SetScreenRedrawFlagToFalse();
            }

            renderer.Render();

            glfwSwapBuffers(window);
        }

        glfwDestroyWindow(window);

        glfwTerminate();
    }
    catch (std::exception &exception)
    {
        std::cout << exception.what() << std::endl;
    }

    return 0;
}