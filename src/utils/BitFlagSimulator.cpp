//
// Created by 13270 on 24-9-24.
//

#include "BitFlagSimulator.h"

template <std::size_t N>
BitFlagSimulator<N>::BitFlagSimulator(std::array<int, N> byte_maximums) : byte_maximums(std::move(byte_maximums))
{
    bytes = new std::array<int, N>(0);
}
template <std::size_t N>
int BitFlagSimulator<N>::operator&(std::bitset<N> rhs)
{
    int index{0};
    std::bitset<N> left{1};
    left = left << N - 1;
    while (rhs != left)
    {
        rhs = rhs << 1;
        ++index;
    }

    return bytes[index];
}
template <std::size_t N>
std::array<int, N> BitFlagSimulator<N>::operator++()
{
    return bytes;
}
template <std::size_t N>
std::array<int, N> BitFlagSimulator<N>::operator++(int)
{
    return bytes;
}
