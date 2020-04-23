#ifndef EMULATOR_H
#define EMULATOR_H

#include "./singleton.h"

enum endian
{
    LITTLE = 0,
    BIG = 0
};

class emulator: public singleton<emulator>
{
public:
    emulator();
    ~emulator();

    bool is_target_little_endian() const;
    bool is_key_pressed(unsigned short key) const;
    void initialize(unsigned char * program, unsigned short program_size);
    void start();
private:
    void set_target_endian_type();

    endian target_endian_type;
};
#endif
