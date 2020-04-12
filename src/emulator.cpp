#include "emulator.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>

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
    is_rom_loaded = false;

    // Chip8 expects the rom to be loaded at memory location 0x200
    program_counter_ = 0x200;
    current_opcode_ = 0;
    index_register_ = 0;
    stack_pointer_ = 0;

    // Clear display
    for (auto i = 0; i < 64 * 32; ++i)
    {
        screen_pixels_[i] = 0;
    }

    // Clear stack
    for (auto i = 0; i < 16; ++i)
    {
        stack_[i] = 0;
    }

    // Clear registers V0-VF
    for (auto i = 0; i < 16; ++i)
    {
        key_[i] = registers_[i] = 0;
    }

    // Clear memory
    for (auto i = 0; i < 4096; ++i)
    {
        memory_[i] = 0;
    }

    // Load fontset
    for (auto i = 0; i < 80; ++i)
    {
        memory_[i] = chip8_fontset[i];
    }

    draw_flag_ = true;

    // Reset timers
    delay_timer_ = 0;
    sound_timer_ = 0;
}

void Emulator::LoadROM(std::string file_path)
{
    if (is_rom_loaded)
    {
        throw std::runtime_error("A ROM is already loaded!");
    }

    std::ifstream rom(file_path, std::ifstream::binary);

    if (!rom.is_open())
    {
        throw std::invalid_argument("Failed to open ROM file: " + file_path + "!");
    }

    std::copy(std::istreambuf_iterator<char>(rom), std::istreambuf_iterator<char>(), &memory_[program_counter_]);

    is_rom_loaded = true;
}

void Emulator::LoadROM(std::vector<unsigned char> raw_data)
{
    if (is_rom_loaded)
    {
        throw std::runtime_error("A ROM is already loaded!");
    }

    std::copy(raw_data.begin(), raw_data.end(), &memory_[program_counter_]);

    is_rom_loaded = true;
}

void Emulator::UnloadROM()
{
    if (!is_rom_loaded)
    {
        return;
    }

    Initialise();
}

void Emulator::EmulateCycle()
{
    unsigned short opcode = AcquireNextOpcode();

    DecodeOpcode(opcode);

    UpdateTimers();
}

unsigned short Emulator::AcquireNextOpcode()
{
    // Opcodes stored in two bytes use a bitwise OR operation to merge the two bits
    // First shift left 8 bits, which adds 8 zeros.  Then merge with OR.
    return memory_[program_counter_] << 8 | memory_[program_counter_ + 1];
}

void Emulator::DecodeOpcode(unsigned short opcode)
{
    // std::cout << "Processing opcode: 0x" << std::uppercase << std::hex << opcode << std::endl;

    switch (opcode & 0xF000)
    {
    case 0x0000:
    {
        switch (opcode & 0x000F)
        {
        case 0x0000: // 0x00E0: Clears the screen
        {
            std::cout << "clear" << std::endl;
            for (auto i = 0; i < 64 * 32; i++)
            {
                screen_pixels_[i] = 0;
            }

            draw_flag_ = true;

            program_counter_ += 2;

            break;
        }
        case 0x000E: // 0x00EE: Returns from subroutine
        {
            // Retreive the last pointer on the stack
            --stack_pointer_;
            program_counter_ = stack_[stack_pointer_];
            program_counter_ += 2;

            break;
        }
        default:
        {
            std::stringstream errorStream;
            errorStream << "Unknown opcode [0x0000]: 0x" << std::uppercase << std::hex << opcode;
            throw std::runtime_error(errorStream.str());
        }
        }

        break;
    }
    case 0x1000: // 0x1NNN: Jumps to address NNN
    {
        program_counter_ = (opcode & 0x0FFF);
        break;
    }
    case 0x2000: // 0x2NNN: Calls a subroutine at address NNN
    {
        stack_[stack_pointer_] = program_counter_;
        ++stack_pointer_;
        program_counter_ = opcode & 0x0FFF;

        break;
    }
    case 0x3000: // 0x3XNN: Skips the next instruction if register X equals NN
    {
        if (registers_[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
        {
            program_counter_ += 4;
        }
        else
        {
            program_counter_ += 2;
        }
        break;
    }
    case 0x4000: // 4XNN: Skips the next instruction if register X doesn't equal NN
    {
        if (registers_[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
        {
            program_counter_ += 4;
        }
        else
        {
            program_counter_ += 2;
        }
        break;
    }
    case 0x6000: // 6XNN: Sets VX to NN
    {
        registers_[(opcode & 0x0F00) >> 8] = (opcode & 0x00FF);
        program_counter_ += 2;

        break;
    }
    case 0x7000: // 7XNN: Adds NN to register X (Carry flag not changed)
    {
        registers_[(opcode & 0x0F00) >> 8] += (opcode & 0x00FF);
        program_counter_ += 2;

        break;
    }
    case 0x8000:
    {
        switch (opcode & 0x000F)
        {
        case 0x0000: // 8XY0: Sets register X to the value of register Y
        {
            registers_[(opcode & 0x0F00) >> 8] = registers_[(opcode & 0x00F0) >> 4];

            program_counter_ += 2;
            break;
        }
        case 0x0001: // 8XY1: Sets register X to VX OR VY
        {
            registers_[(opcode & 0x0F00) >> 8] |= registers_[(opcode & 0x00F0) >> 4];

            program_counter_ += 2;

            break;
        }
        case 0x0002: // 8XY2: Sets register X to register X AND register Y
        {
            registers_[(opcode & 0x0F00) >> 8] &= registers_[(opcode & 0x00F0) >> 4];

            program_counter_ += 2;

            break;
        }
        case 0x0003: // 8XY3: Sets register X to register X XOR register Y
        {
            registers_[(opcode & 0x0F00) >> 8] ^= registers_[(opcode & 0x00F0) >> 4];

            program_counter_ += 2;
            break;
        }
        case 0x0004: // 8XY4: Adds the value of register Y to register X. Register F is set to 1 when there is a carry
        {
            if (registers_[(opcode & 0x00F0) >> 4] > (0xFF - registers_[(opcode & 0x0F00) >> 8]))
            {
                registers_[0xF] = 1; // Carry
            }
            else
            {
                registers_[0xF] = 0;
            }
            registers_[(opcode & 0x0F00) >> 8] += registers_[(opcode & 0x00F0) >> 4];
            program_counter_ += 2;

            break;
        }
        case 0x0005: // 8XY5: register Y is subtracted from register X, register F is set to 0 when there's a borrow, and 1 when there isn't
        {
            if (registers_[(opcode & 0x00F0) >> 4] > registers_[(opcode & 0x0F00) >> 8])
            {
                registers_[0xF] = 0;
            }
            else
            {
                registers_[0xF] = 1;
            }
            registers_[(opcode & 0x0F00) >> 8] -= registers_[(opcode & 0x00F0) >> 4];

            program_counter_ += 2;
            break;
        }
        default:
        {
            std::stringstream errorStream;
            errorStream << "Unknown opcode [0x8000]: 0x" << std::uppercase << std::hex << opcode;
            throw std::runtime_error(errorStream.str());
        }
        }
        break;
    }
    case 0x9000: // 9XY0: Skips the next instruction if register X doen't equal register Y
    {
        if (registers_[(opcode & 0x0F00) >> 8] != registers_[(opcode & 0x00F0) >> 4])
        {
            program_counter_ += 4;
        }
        else
        {
            program_counter_ += 2;
        }
        break;
    }
    case 0xA000: // ANNN: Sets index_register_ to the address NNN
    {
        // Execute opcode
        index_register_ = opcode & 0x0FFF;
        program_counter_ += 2;
        break;
    }
    case 0xB000: // BNNN: Jumps to the address of NNN plus register 0
    {
        program_counter_ = (opcode & 0x0FFF) + registers_[0];

        break;
    }
    case 0xC000: // CXNN: Sets register X to the result of a bitwise and operation on a random number and NN
    {
        std::ranlux24 gen;
        std::uniform_int_distribution<int> random_number(0, 255);
        registers_[(opcode & 0x0F00) >> 8] = random_number(gen) & (opcode & 0x00FF);

        program_counter_ += 2;

        break;
    }
    case 0xD000: // 0xDXYN: Drawing to the display, X and Y are coordinates of the sprite, N is number of rows
    {
        unsigned short x = registers_[(opcode & 0x0F00) >> 8];
        unsigned short y = registers_[(opcode & 0x00F0) >> 4];
        unsigned short n = opcode & 0x000F;

        registers_[0xF] = 0;
        for (auto row_index = 0; row_index < n; row_index++)
        {
            unsigned char row = memory_[index_register_ + row_index];

            // Don't try and fill pixels outside the frame buffer
            if (y + row_index >= 32)
            {
                continue;
            }

            for (auto pixel_index = 0; pixel_index < 8; pixel_index++)
            {
                // Don't try to fill pixels outside the frame buffer
                if (x + pixel_index >= 64)
                {
                    continue;
                }

                // TODO: swap this
                if ((row & (0x80 >> pixel_index)) != 0)
                {
                    int index = (x + pixel_index + ((y + row_index) * 64));
                    if (screen_pixels_[index] == 1)
                    {
                        registers_[0xF] = 1;
                    }
                    screen_pixels_[x + pixel_index + ((y + row_index) * 64)] ^= 1;
                }
            }
        }

        draw_flag_ = true;
        program_counter_ += 2;

        break;
    }
    case 0xE000:
    {
        switch (opcode & 0x00FF)
        {
        case 0x009E: // EX9E: Skips the next instruction if the key stored in register X is pressed
        {
            if (key_[registers_[(opcode & 0x0F00) >> 8]] != 0)
            {
                program_counter_ += 4;
            }
            else
            {
                program_counter_ += 2;
            }
            break;
        }
        case 0x00A1: // EXA1: Skips the next instruction if the key stored in register X isn't pressed.
        {
            if (key_[registers_[(opcode & 0x0F00) >> 8]] == 0)
            {
                program_counter_ += 4;
            }
            else
            {
                program_counter_ += 2;
            }
            break;
        }
        default:
        {
            std::stringstream errorStream;
            errorStream << "Unknown opcode [0xE000]: 0x" << std::uppercase << std::hex << opcode;
            throw std::runtime_error(errorStream.str());
        }
        }
        break;
    }
    case 0xF000:
    {
        switch (opcode & 0x00FF)
        {
        case 0x0007: // FX07: Sets the register X the value of the delay timer
        {
            registers_[(opcode & 0x0F00) >> 8] = delay_timer_;

            program_counter_ += 2;

            break;
        }
        case 0x000A: // 0xFX0A: stores the name of a key on keypress in register X
        {
            bool key_pressed = false;

            for (auto i = 0; i < 16; i++)
            {
                if (key_[i] != 0)
                {
                    registers_[(opcode & 0x0F00) >> 8] = i;
                    key_pressed = true;
                }
            }

            // If a key has not been pressed, skip this cycle and try again
            if (!key_pressed)
            {
                return;
            }

            program_counter_ += 2;

            break;
        }
        case 0x0015: // FX15: Sets the delay timer to register X
        {
            delay_timer_ = registers_[(opcode & 0x0F00) >> 8];

            program_counter_ += 2;

            break;
        }
        case 0x0018: // FX18: Sets the sound timer to register X
        {
            sound_timer_ = registers_[(opcode & 0x0F00) >> 8];

            program_counter_ += 2;

            break;
        }
        case 0x001E: // FX1E: Adds register X to index_register_. Register F is set to 1 when there is a range overflow, and 0 when there isn't
        {
            unsigned char register_x = registers_[(opcode & 0x0F00) >> 8];
            if (index_register_ + register_x > 0xFFF)
            {
                registers_[0xF] = 1; // Carry
            }
            else
            {
                registers_[0xF] = 0;
            }

            index_register_ += register_x;

            program_counter_ += 2;
            break;
        }
        case 0x0029: // FX29: Sets index_register_ to the location of the sprite for the character in register X
        {
            index_register_ = registers_[(opcode & 0x0F00) >> 8] * 0x5;

            program_counter_ += 2;

            break;
        }
        case 0x0033: // FX33:  Stores the Binary-coded decimal representation of register X at the addresses I, I plus 1, and I plus 2
        {
            memory_[index_register_] = registers_[(opcode & 0x0F00) >> 8] / 100;
            memory_[index_register_ + 1] = (registers_[(opcode & 0x0F00) >> 8] / 10) % 10;
            memory_[index_register_ + 2] = (registers_[(opcode & 0x0F00) >> 8] % 100) % 10;
            program_counter_ += 2;

            break;
        }
        case 0x0065: // FX65: Fills register 0 to register X in memory starting at address index_register_
        {
            for (auto i = 0; i<(opcode & 0x0F00)>> 8; i++)
            {
                registers_[i] = memory_[index_register_ + i];
            }

            program_counter_ += 2;

            break;
        }
        default:
        {
            std::stringstream errorStream;
            errorStream << "Unknown opcode [0xF000]: 0x" << std::uppercase << std::hex << opcode;
            throw std::runtime_error(errorStream.str());
        }
        }
        break;
    }
    default:
    {
        std::stringstream errorStream;
        errorStream << "Unknown opcode: 0x" << std::uppercase << std::hex << opcode;
        throw std::runtime_error(errorStream.str());
    }
    }
} // namespace chip8emu

void Emulator::UpdateTimers()
{
    if (delay_timer_ > 0)
    {
        --delay_timer_;
    }
    if (sound_timer_ > 0)
    {
        if (sound_timer_ == 1)
        {
            std::cout << "BEEP" << std::endl;
            --sound_timer_;
        }
    }
}

void Emulator::KeyPressed(KeyCode key_code)
{
    key_[key_code] = 1;
}

void Emulator::KeyReleased(KeyCode key_code)
{
    key_[key_code] = 0;
}

bool Emulator::DoesScreenNeedRedraw()
{
    return draw_flag_;
}

void Emulator::SetScreenRedrawFlagToFalse()
{
    draw_flag_ = false;
}

void Emulator::DebugDraw()
{
    for (auto y = 0; y < 32; y++)
    {
        for (auto x = 0; x < 64; x++)
        {
            if (screen_pixels_[x + (y * 64)] != 0)
            {
                std::cout << "X";
            }
            else
            {
                std::cout << "0";
            }
        }
        std::cout << std::endl;
    }
}
} // namespace chip8emu