#include <cstdlib>
#include <ctime>
#include <cstdio>

#include "./cpu.h"
#include "./mmu.h"
#include "./gpu.h"
#include "./emulator.h"

template<>
cpu * singleton<cpu>::instance = nullptr;

unsigned short cpu::run(unsigned short cycles)
{
    for (int i = 0; i < cycles; ++ i)
    {
        unsigned short opcode = mmu::get_instance().fetch(pc);
        decode_data dd = decode(opcode);
        execute(dd);
    }   

    return 0;
}

decode_data cpu::decode(unsigned short opcode) const
{
    switch (opcode & 0xF000)
    {
        case 0x0000:
        {
            switch (opcode & 0x00FF)
            {
                case 0x00E0:
                    return {CLS};
                case 0x00EE:
                    return {RET};
            }

            unsigned short address = extract_address(opcode);
            return {SYS, address};
        }
        case 0x1000:
        {
            unsigned short address = extract_address(opcode);
            return {JP, address};
        }
        case 0x2000:
        {
            unsigned short address = extract_address(opcode);
            return {CALL, address};
        }
        case 0x3000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char byte = extract_byte(opcode);
            return {SE_BYTE, 0, x, 0, byte};
        }
        case 0x4000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char byte = extract_byte(opcode);
            return {SNE_BYTE, 0, x, 0, byte};
        }
        case 0x5000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char y = extract_y(opcode);
            return {SE, 0, x, y};
        }
        case 0x6000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char byte = extract_byte(opcode);
            return {LD_BYTE, 0, x, 0, byte};
        }
        case 0x7000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char byte = extract_byte(opcode);
            return {ADD_BYTE, 0, x, 0, byte};
        }
        case 0x8000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char y = extract_y(opcode);
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    return {LD, 0, x, y};
                case 0x0001:
                    return {OR, 0, x, y};
                case 0x0002:
                    return {AND, 0, x, y};
                case 0x0003:
                    return {XOR, 0, x, y};
                case 0x0004:
                    return {ADD, 0, x, y};
                case 0x0005:
                    return {SUB, 0, x, y};
                case 0x0006:
                    return {SHR, 0, x, y};
                case 0x0007:
                    return {SUBN, 0, x, y};
                case 0x000E:
                    return {SHL, 0, x, y};
            }
            break;
        }
        case 0x9000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char y = extract_y(opcode);
            return {SNE, 0, x, y};
        }
        case 0xA000:
        {
            unsigned short address = extract_address(opcode);
            return {LDI, address};
        }
        case 0xB000:
        {
            unsigned short address = extract_address(opcode);
            return {JPV0, address};
        }
        case 0xC000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char byte = extract_byte(opcode);
            return {RND, 0, x, 0, byte};
        }
        case 0xD000:
        {
            unsigned char x = extract_x(opcode);
            unsigned char y = extract_y(opcode);
            unsigned char nibble = extract_nibble(opcode);
            return {DRW, 0, x, y, 0, nibble};
        }
        case 0xE000:
        {
            unsigned char x = extract_x(opcode);
            switch (opcode & 0x00FF)
            {
                case 0x009E:
                    return {SKP, 0, x};
                case 0x00A1:
                    return {SKNP, 0, x};
            }
            break;
        }
        case 0xF000:
        {
            unsigned char x = extract_x(opcode);
            switch (opcode & 0x00FF)
            {
                case 0x0007:
                    return {FX07, 0, x};
                case 0x000A:
                    return {FX0A, 0, x};
                case 0x0015:
                    return {FX15, 0, x};
                case 0x0018:
                    return {FX18, 0, x};
                case 0x001E:
                    return {FX1E, 0, x};
                case 0x0029:
                    return {FX29, 0, x};
                case 0x0033:
                    return {FX33, 0, x};
                case 0x0055:
                    return {FX55, 0, x};
                case 0x0065:
                    return {FX65, 0, x};
            }
            break;
        }
    }
    return {RET};
}

void cpu::execute(decode_data data)
{
    switch (data.type)
    {
        case SYS:
        {
            pc = data.address;
            break;
        }
        case CLS:
        {
            gpu::get_instance().clear_screen();
            break;
        }
        case RET:
        {
            pc = st.top();
            st.pop();
            break;
        }
        case JP:
        {
            pc = data.address;
            break;
        }
        case CALL:
        {
            st.push(pc);
            pc = data.address;
            break;
        }
        case SE_BYTE:
        {
            if (registers[data.x] == data.byte)
                pc ++;
            break;
        }
        case SNE_BYTE:
        {
            if (registers[data.x] != data.byte)
                pc ++;
            break;
        }
        case SE:
        {
            if (registers[data.x] == registers[data.y])
                pc ++;
            break;
        }
        case LD_BYTE:
        {
            registers[data.x] = data.byte;
            break;
        }
        case ADD_BYTE:
        {
            registers[data.x] += data.byte;
            break;
        }
        case LD:
        {
            registers[data.x] = registers[data.y];
            break;
        }
        case OR:
        {
            registers[data.x] |= registers[data.y];
            break;
        }
        case AND:
        {
            registers[data.x] &= registers[data.y];
            break;
        }
        case XOR:
        {
            registers[data.x] ^= registers[data.y];
            break;
        }
        case ADD:
        {
            unsigned short res = (unsigned short)registers[data.x] + (unsigned short)registers[data.y];
            registers[data.x] = res > 255 ? 255 : res;
            registers[15] = res > 255 ? 1 : 0;
            break;
        }
        case SUB:
        {
            unsigned short res = (unsigned short)registers[data.x] - (unsigned short)registers[data.y];
            registers[15] = registers[data.x] > registers[data.y] ? 1 : 0;
            registers[data.x] = res;
            break;
        }
        case SHR:
        {
            registers[15] = registers[data.x] % 2;
            registers[data.x] /= 2;
            break;
        }
        case SUBN:
        {
            unsigned short res = (unsigned short)registers[data.x] - (unsigned short)registers[data.y];
            registers[15] = registers[data.y] > registers[data.x] ? 1 : 0;
            registers[data.x] = res;
            break;
        }
        case SHL:
        {
            registers[15] = (registers[data.x] >> 7) % 2;
            registers[data.x] *= 2;
            break;
        }
        case SNE:
        {
            if (registers[data.x] != registers[data.y])
                pc ++;
            break;
        }
        case LDI:
        {
            i = data.address;
            break;
        }
        case JPV0:
        {
            pc = data.address + registers[0];
            break;
        }
        case RND:
        {
            unsigned char r = (unsigned char)(std::rand() % 256);
            registers[data.x] = r & data.byte;
            break;
        }
        case DRW:
        {
            unsigned char * buffer = mmu::get_instance().fetch_bytes(i, data.nibble);
            registers[15] = gpu::get_instance().add_to_screen(buffer, data.nibble, data.x, data.y);
            break;
        }
        case SKP:
        {
            if (emulator::get_instance().is_key_pressed(data.x))
                ++ pc;
            break;
        }
        case SKNP:
        {
            if (!emulator::get_instance().is_key_pressed(data.x))
                ++ pc;
            break;
        }
        case FX07:
        {
            registers[data.x] = delay_timer;
            break;
        }
        case FX0A:
        {
            registers[data.x] = std::getchar();
            break;
        }
        case FX15:
        {
            delay_timer = registers[data.x];
            break;
        }
        case FX18:
        {
            sound_timer = registers[data.x];
            break;
        }
        case FX1E:
        {
            i += registers[data.x];
            break;
        }
        case FX29:
        {
            i = 5 * registers[data.x];
            break;
        }
        case FX33:
        {
            mmu::get_instance().store(i, registers[data.x] / 100);
            mmu::get_instance().store(i + 1, (registers[data.x] / 10) % 10);
            mmu::get_instance().store(i + 2, registers[data.x] % 10);
            break;
        }
        case FX55:
        {
            for (int idx = 0; idx <= registers[data.x]; ++ idx)
            {
                mmu::get_instance().store(i + idx, registers[idx]);
            }
            break;
        }
        case FX65:
        {
            for (int idx = 0; idx <= data.x; ++ idx)
            {
                registers[idx] = mmu::get_instance().fetch_byte(i + idx);
            }
            break;
        }
    }
}

unsigned short cpu::extract_address(unsigned short opcode) const
{
    return opcode & 0x0FFF;
}

unsigned char cpu::extract_x(unsigned short opcode) const
{
    return (opcode & 0x0F00) >> 2;
}

unsigned char cpu::extract_y(unsigned short opcode) const
{
    return (opcode & 0x00F0) >> 1;
}

unsigned char cpu::extract_byte(unsigned short opcode) const
{
    return opcode & 0x00FF;
}

unsigned char cpu::extract_nibble(unsigned short opcode) const
{
    return opcode & 0x000F;
}

void cpu::decrement_delay_timer()
{
    if (delay_timer > 0)
        delay_timer --;
}

void cpu::decrement_sound_timer()
{
    if (sound_timer > 0)
        sound_timer --;
}
