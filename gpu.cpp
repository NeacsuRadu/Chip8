#include "./gpu.h"

template<>
gpu * singleton<gpu>::instance = nullptr;

gpu::gpu()
{
   clear_screen();
}

void gpu::clear_screen()
{
    for (int l = 0; l < 32; ++ l)
        for (int c = 0; c < 64; ++ c)
            screen[l][c] = 0;
}

unsigned char gpu::add_to_screen(unsigned char * buffer, unsigned short length, unsigned short x, unsigned short y)
{
    unsigned char pixels_were_erased = 0;

    for (int index = 0; index < length * 8; ++ index)
    {
       unsigned short l = (y + (index / 8)) % 32;
       unsigned short c = (x + (index % 8)) % 64;
       unsigned char prev = screen[l][c];
       if (screen[l][c] ^ buffer[index] == 0)
           pixels_were_erased = 1;
        
       screen[l][c] ^= buffer[index];
    }

    return pixels_were_erased;
}
