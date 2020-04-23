#include "./emulator.h"

template<>
emulator * singleton<emulator>::instance = nullptr;

emulator::emulator()
{}

emulator::~emulator()
{}

bool emulator::is_target_little_endian() const
{
    return target_endian_type == LITTLE;
}

bool emulator::is_key_pressed(unsigned short key) const
{
    return true;
}

void emulator::start()
{}

void emulator::initialize(unsigned char * program, unsigned short program_size)
{
    set_target_endian_type();
}

void emulator::set_target_endian_type()
{
    unsigned short buffer = 1;
    unsigned char high_byte = ((unsigned char *)&buffer)[0];
    if (high_byte == 1)
        target_endian_type = LITTLE;
    else
        target_endian_type = BIG;   
}
