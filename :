#ifndef MMU_H
#define MMU_H

#include "./singleton.h"

class mmu: public singleton<mmu>
{
public:
    mmu();
    ~mmu();
    
    void set_memory(unsigned char * buffer, unsigned short size);
    unsigned short fetch(unsigned short address) const;
    unsigned char fetch_byte(unsigned short address) const;
    unsigned char * fetch_bytes(unsigned short address, unsigned short length) const;
    void store(unsigned short address, unsigned char byte);
private:
    unsigned char * memory = nullptr;
};
#endif
