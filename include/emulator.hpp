#ifndef EMULATOR_HPP_
#define EMULATOR_HPP_

namespace chip8emu
{
class Emulator
{
public:
    Emulator();
    ~Emulator();

private:
    unsigned char memory_[4096];
    // V
    unsigned char registers_[16];
    // I
    // Range: 0x000 - 0xFFF
    unsigned short index_register_;
    // pc
    // Range 0x000 - 0xFFF
    unsigned short program_counter_;
    // gfx
    unsigned char screen_pixels_[64 * 32];
    // Two timer registers (delay_timer_ and sound_timer_) that count at 60 Hz.
    // When set above zero they will count down to zero.
    unsigned char delay_timer_;
    // The system buzzer sounds whenever sound_timer_ reaches zero
    unsigned char sound_timer_;
    // Stores the current memory location before a jump is performed
    unsigned short stack_[16];
    // sp
    unsigned short stack_pointer_;
    // Keypad
    unsigned char key_[16];
};
} // namespace chip8emu

#endif // EMULATOR_HPP_