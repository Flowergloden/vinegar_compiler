//
// Created by 13270 on 24-9-28.
//
#include "BitFlagSimulator.h"

int BitFlagSimulator::operator&(std::bitset<MAX_LENGTH> rhs) const
{
    int index{0};
    std::bitset<MAX_LENGTH> left{1};
    left = left << length - 1;
    while (rhs != left)
    {
        rhs = rhs << 1;
        ++index;
    }

    return bytes[index];
}

std::vector<int> BitFlagSimulator::operator++()
{
    for (int i = static_cast<int>(length) - 1; i >= 0; --i)
    {
        if (bytes[i] < byte_maximums[i])
        {
            ++bytes[i];
            break;
        }
    }

    return bytes;
}

std::vector<int> BitFlagSimulator::operator++(int)
{
    auto prev_bytes = bytes;

    for (int i = static_cast<int>(length) - 1; i >= 0; --i)
    {
        if (bytes[i] < byte_maximums[i])
        {
            ++bytes[i];
            break;
        }
    }

    return prev_bytes;
}
