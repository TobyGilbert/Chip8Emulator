#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

#include <array>
#include <vector>

namespace chip8emu
{
class Emulator
{
public:
    Emulator();
    ~Emulator();

    void LoadROM(std::string file_path);
    void LoadROM(std::vector<unsigned char> raw_data);
    void UnloadROM();
    void EmulateCycle();

    enum KeyCode
    {
        KEYCODE_X,
        KEYCODE_1,
        KEYCODE_2,
        KEYCODE_3,
        KEYCODE_Q,
        KEYCODE_W,
        KEYCODE_E,
        KEYCODE_A,
        KEYCODE_S,
        KEYCODE_D,
        KEYCODE_Z,
        KEYCODE_C,
        KEYCODE_4,
        KEYCODE_R,
        KEYCODE_F,
        KEYCODE_V
    };

    void KeyPressed(KeyCode key_code);
    void KeyReleased(KeyCode key_code);
    bool DoesScreenNeedRedraw();
    void SetScreenRedrawFlagToFalse();
    inline std::array<unsigned char, 64 * 32> GetScreenPixels() { return screen_pixels_; }
    void DebugDraw();

private:
    void Initialise();
    /// Returns the next opcode
    unsigned short AcquireNextOpcode();
    void DecodeOpcode(unsigned short opcode);
    void UpdateTimers();

    std::array<unsigned char, 4096> memory_ = {};
    // V
    std::array<unsigned char, 16> registers_ = {};
    // I
    // Range: 0x000 - 0xFFF
    unsigned short index_register_ = 0;
    // pc
    // Range 0x000 - 0xFFF
    unsigned short program_counter_ = 0;
    // gfx
    std::array<unsigned char, 64 * 32> screen_pixels_ = {};
    bool draw_flag_ = false;
    // Two timer registers (delay_timer_ and sound_timer_) that count at 60 Hz.
    // When set above zero they will count down to zero.
    unsigned char delay_timer_;
    // The system buzzer sounds whenever sound_timer_ reaches zero
    unsigned char sound_timer_;
    // Stores the current memory location before a jump is performed
    std::array<unsigned short, 16> stack_ = {};
    // sp
    unsigned short stack_pointer_;
    // Keypad
    std::array<unsigned char, 16> key_ = {};
    unsigned short current_opcode_;

    std::array<unsigned char, 80> chip8_fontset = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    bool is_rom_loaded = false;
};
} // namespace chip8emu

#endif // EMULATOR_HPP_