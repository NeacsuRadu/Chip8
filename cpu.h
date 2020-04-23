#ifndef CPU_H
#define CPU_H

#include <stack>

#include "./singleton.h"

enum instruction
{
    SYS,
    CLS,
    RET,
    JP,
    CALL,
    SE_BYTE,
    SNE_BYTE,
    SE,
    LD_BYTE,
    ADD_BYTE,
    LD,
    OR,
    AND,
    XOR,
    ADD,
    SUB,
    SHR,
    SUBN,
    SHL,
    SNE,
    LDI,
    JPV0,
    RND,
    DRW,
    SKP,
    SKNP,
    FX07,
    FX0A,
    FX15,
    FX18,
    FX1E,
    FX29,
    FX33,
    FX55,
    FX65
};

struct decode_data
{
    instruction type;
    unsigned short address = 0;
    unsigned char x = 0;
    unsigned char y = 0; 
    unsigned char byte = 0;
    unsigned char nibble = 0;
};

class cpu: public singleton<cpu>
{
public:
    cpu() = default;

    unsigned short run(unsigned short cycles);    
    void decrement_delay_timer();
    void decrement_sound_timer();
private:
    decode_data decode(unsigned short opcode) const;
    unsigned short extract_address(unsigned short opcode) const;
    unsigned char extract_x(unsigned short opcode) const;
    unsigned char extract_y(unsigned short opcode) const;
    unsigned char extract_byte(unsigned short opcode) const;
    unsigned char extract_nibble(unsigned short opcode) const;
    void execute(decode_data data);

    unsigned char registers[16];
    unsigned short i;
    unsigned short pc;
    unsigned char delay_timer;
    unsigned char sound_timer;
    std::stack<unsigned short> st;
};
#endif
