#ifndef GPU_H
#define GPU_H

#include "./singleton.h"

class gpu: public singleton<gpu>
{
public:
    gpu();

    void draw();
    void clear_screen();
    unsigned char add_to_screen(unsigned char * buffer, unsigned short length, unsigned short x, unsigned short y);
private:
    unsigned char screen[32][64];
};
#endif
