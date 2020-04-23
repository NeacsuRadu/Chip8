#include <cstring>

#include "./mmu.h"
#include "./emulator.h"

template<>
mmu * singleton<mmu>::instance = nullptr;

mmu::mmu()
{
    memory = new unsigned char[4096]();
    
    unsigned char digits[] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xF0, 0x10, 0xF0, 0x80, 0xF0,
        0xF0, 0x10, 0xF0, 0x10, 0xF0,
        0x90, 0x90, 0xF0, 0x10, 0x10,
        0xF0, 0x80, 0xF0, 0x10, 0xF0,
        0xF0, 0x80, 0xF0, 0x90, 0xF0,
        0xF0, 0x10, 0x20, 0x40, 0x40,
        0xF0, 0x90, 0xF0, 0x90, 0xF0,
        0xF0, 0x90, 0xF0, 0x10, 0xF0,
        0xF0, 0x90, 0xF0, 0x90, 0x90,
        0xE0, 0x90, 0xE0, 0x90, 0xE0,
        0xF0, 0x80, 0x80, 0x80, 0xF0,
        0xE0, 0x90, 0x90, 0x90, 0xE0,
        0xF0, 0x80, 0xF0, 0x80, 0xF0,
        0xF0, 0x80, 0xF0, 0x80, 0x80
    };

    std::memcpy(memory, digits, 5 * 16);
}

mmu::~mmu()
{
    delete[] memory;
}

void mmu::set_memory(unsigned char * buffer, unsigned short size)
{
    std::memcpy(memory + 512, buffer, size);   
}

unsigned short mmu::fetch(unsigned short address) const
{
    unsigned short opcode = 0;
    unsigned char * opcode_buffer = (unsigned char *)&opcode;
    if (emulator::get_instance().is_target_little_endian())
    {
        opcode_buffer[0] = memory[address + 1];
        opcode_buffer[1] = memory[address];
    }
    else
    {
        opcode_buffer[0] = memory[address];
        opcode_buffer[1] = memory[address + 1];
    }

    return opcode;
}

void mmu::store(unsigned short address, unsigned char byte)
{
    memory[address] = byte;
}

unsigned char mmu::fetch_byte(unsigned short address) const
{
    return memory[address];
}

unsigned char * mmu::fetch_bytes(unsigned short address, unsigned short length) const
{
    return memory + address;
}
