#include "emulator.hpp"

#include <fstream>

namespace chip8emu
{
Emulator::Emulator()
{
    Initialise();
}

Emulator::~Emulator()
{
}

void Emulator::Initialise()
{
    // Chip8 expects the rom to be loaded at memory location 0x200
    program_counter_ = 0x200;
    current_opcode_ = 0;
    index_register_ = 0;
    stack_pointer_ = 0;

    // Clear display
    // Clear stack
    // Clear registers V0-VF
    // Clear memory

    // Load fontset
    for (auto i = 0; i < 80; ++i)
    {
        memory_[i] = chip8_fontset[i];
    }

    // Reset timers
}

void Emulator::LoadROM(std::string file_path)
{
    std::ifstream rom(file_path, std::ifstream::binary);

    if (!rom.is_open())
    {
        throw std::invalid_argument("Failed to open ROM file: " + file_path + "!");
    }
}

void Emulator::EmulateCycle()
{
    // Fetch opcode
    // Opcodes stored in two bytes use a bitwise OR operation to merge the two bits
    // First shift left 8 bits, which adds 8 zeros.  Then merge with OR.
    unsigned short opcode = memory_[program_counter_] << 8 | memory_[program_counter_ + 1];

    // Decode opcode
    // Execute opcode

    // Update timers
}
} // namespace chip8emu