#include "renderer.hpp"
#include <glad/glad.h>

#include <iostream>
#include <vector>

namespace chip8emu
{
Renderer::Renderer()
{
    Initialise();
}

Renderer::~Renderer()
{
    Destroy();
}

void Renderer::Resize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer::UpdateTexture(unsigned char *data)
{
    glBindTexture(GL_TEXTURE_2D, screen_texture_);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 64, 32, GL_RED, GL_UNSIGNED_BYTE, data);
}

void Renderer::Render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader_program_);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screen_texture_);
    glBindVertexArray(VAO_);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::Initialise()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    // Create screen space quad
    glGenVertexArrays(1, &VAO_);
    glBindVertexArray(VAO_);

    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f,
        1.0, 1.0f, 0.0f};

    glGenBuffers(1, &VBO_);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
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
    glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);
    glCompileShader(vertex_shader);

    const char *fragment_shader_source = "#version 330 core\n"
                                         "uniform sampler2D screen_texture;\n"
                                         "in vec2 texture_coordinate;"
                                         "out vec4 FragColor;\n"
                                         "vec2 curve(vec2 uv)\n"
                                         "{\n"
                                         "   uv = (uv - 0.5) * 2.0;\n"
                                         "   uv *= 1.1;\n"
                                         "   uv.x *= 1.0 + pow((abs(uv.y) / 5.0), 2.0);\n"
                                         "   uv.y *= 1.0 + pow((abs(uv.x) / 4.0), 2.0);\n"
                                         "   uv  = (uv / 2.0) + 0.5;\n"
                                         "   uv =  uv *0.92 + 0.04;\n"
                                         "   return uv;\n"
                                         "}\n"
                                         "void main()\n"
                                         "{\n"
                                         "    vec2 uv = curve(texture_coordinate);"
                                         "    float emulator = texture(screen_texture, uv).r;"
                                         "    int subPixelIndex = int(gl_FragCoord.x) % 3;"
                                         "    float r = mix(1.0, 0.0, step(0.001, subPixelIndex));"
                                         "    float b = mix(0.0, 1.0, step(1.9999, subPixelIndex));"
                                         "    vec3 subpixels = vec3(r, 1.0 - max(r, b), b);"
                                         "    vec3 col = vec3(emulator);\n"
                                         "    float iTime = 0.0;\n"
                                         "    vec2 res = vec2(640, 320);"
                                         "    col *= vec3(0.95,1.05,0.95);\n"
                                         "    col *= 2.8;\n"
                                         "    float scans = clamp( 0.35+0.35*sin(3.5*iTime+uv.y*res.y*1.5), 0.0, 1.0);\n"
                                         "    float s = pow(scans,1.7);\n"
                                         "    col = col * vec3(0.4 + 0.7 * s);\n "
                                         "    float vig = (0.0 + 1.0*16.0*uv.x*uv.y*(1.0-uv.x)*(1.0-uv.y));\n"
                                         "    vig = pow(vig, 0.3);\n"
                                         "    vig *= step(0.0, uv.x) * (1.0 - step(1.0, uv.x));\n"
                                         "    vig *= step(0.0, uv.y) * (1.0 - step(1.0, uv.y));\n"
                                         "    col *= vig;\n"
                                         "    col*=1.0-0.65*vec3(clamp((mod(gl_FragCoord.x, 2.0)-1.0)*2.0,0.0,1.0));\n"
                                         "    FragColor = vec4(col, 1.0);\n"
                                         "    //FragColor = vec4(uv, 0.0, 1.0);\n"
                                         "}\0 ";

    unsigned int fragment_shader;
    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);
    glCompileShader(fragment_shader);

    GLint success = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        GLint maxLength = 0;
        glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        GLchar *log_string = new char[maxLength + 1];
        glGetShaderInfoLog(fragment_shader, maxLength, &maxLength, log_string);

        std::cout << log_string << std::endl;
    }

    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vertex_shader);
    glAttachShader(shader_program_, fragment_shader);
    glLinkProgram(shader_program_);
    glUseProgram(shader_program_);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Texture to write chip8 screen to
    glGenTextures(1, &screen_texture_);
    glBindTexture(GL_TEXTURE_2D, screen_texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    float borderColor[] = {1.0f, 1.0f, 0.0f, 1.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 64, 32, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);
    glGenerateMipmap(GL_TEXTURE_2D);

    glEnable(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screen_texture_);
    glUniform1i(glGetUniformLocation(shader_program_, "screen_texture"), 0);
}

void Renderer::Destroy()
{
    glDeleteTextures(1, &screen_texture_);
    glDeleteProgram(shader_program_);
    glDeleteVertexArrays(1, &VAO_);
    glDeleteBuffers(1, &VBO_);
}

} // namespace chip8emu
