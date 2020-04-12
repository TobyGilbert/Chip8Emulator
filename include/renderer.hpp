#ifndef RENDERER_HPP_
#define RENDERER_HPP_

namespace chip8emu
{
class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Resize(int width, int height);
    void UpdateTexture(unsigned char *data);
    void Render();

private:
    Renderer(const Renderer &other);
    Renderer &operator=(const Renderer &other);

    void Initialise();
    void Destroy();

    unsigned int VAO_;
    unsigned int VBO_;

    unsigned int shader_program_;
    unsigned int screen_texture_;
};
} // namespace chip8emu

#endif