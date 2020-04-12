#include <iostream>
#include <chrono>
#include <thread>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "emulator.hpp"

const std::string rom_file_path = "roms/brix.c8";

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    auto emulator = reinterpret_cast<chip8emu::Emulator *>(glfwGetWindowUserPointer(window));

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
    glViewport(0, 0, width, height);
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

        glViewport(0, 0, 64 * 10, 32 * 10);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

        // Create screen space quad
        unsigned int VAO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        float vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0, 1.0f, 0.0f};
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        // Screen shaders
        const char *vertex_shader_source = "#version 410\n"
                                           "layout(location = 0) in vec3 position;\n"
                                           "out vec2 texture_coordinate;"
                                           "void main()\n"
                                           "{\n"
                                           "    gl_Position = vec4(position, 1.0);\n"
                                           "    texture_coordinate = position.xy * 0.5 + 0.5;"
                                           "}\0";
        unsigned int vertex_shader;
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(vertex_shader);

        const char *fragment_shader_source = "#version 330 core\n"
                                             "uniform sampler2D screen_texture;\n"
                                             "in vec2 texture_coordinate;"
                                             "out vec4 FragColor;\n"
                                             "void main()\n"
                                             "{\n"
                                             "    FragColor = texture(screen_texture, texture_coordinate);\n"
                                             "}\0 ";

        unsigned int fragment_shader;
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(fragment_shader);

        unsigned int shader_program;
        shader_program = glCreateProgram();
        glAttachShader(shader_program, vertex_shader);
        glAttachShader(shader_program, fragment_shader);
        glLinkProgram(shader_program);
        glUseProgram(shader_program);

        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);

        // Texture to write chip8 screen to
        unsigned int screen_texture;
        glGenTextures(1, &screen_texture);
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 64, 32, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glGenerateMipmap(GL_TEXTURE_2D);

        glEnable(GL_TEXTURE_2D);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, screen_texture);
        glUniform1i(glGetUniformLocation(shader_program, "screen_texture"), 0);

        // Initialise the Chip8 system and load the game into memory
        chip8emu::Emulator emulator;
        glfwSetWindowUserPointer(window, &emulator);

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
                glBindTexture(GL_TEXTURE_2D, screen_texture);
                glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RED, GL_UNSIGNED_BYTE, data);

                emulator.SetScreenRedrawFlagToFalse();
            }

            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(shader_program);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, screen_texture);
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            glfwSwapBuffers(window);
        }

        glDeleteTextures(1, &screen_texture);
        glDeleteProgram(shader_program);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);

        glfwDestroyWindow(window);

        glfwTerminate();
    }
    catch (std::exception &exception)
    {
        std::cout << exception.what() << std::endl;
    }

    return 0;
}