#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "./emulator.h"

int main(int argc, char * argv[])
{
    for (int i = 0; i < argc; ++ i)
    {
        std::cout << argv[i] << std::endl;
    }

    std::ifstream input(argv[1], std::ios::binary);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    emulator::get_instance().initialize(buffer.data(), buffer.size());
    if (emulator::get_instance().is_target_little_endian())
        std::cout << "Is little endian" << std::endl;
    else
        std::cout << "Is big endian" << std::endl;
    return 0;
}
