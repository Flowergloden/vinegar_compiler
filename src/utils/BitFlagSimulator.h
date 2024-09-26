//
// Created by 13270 on 24-9-24.
//

#ifndef BITFLAGSIMULATOR_H
#define BITFLAGSIMULATOR_H
#include <array>
#include <bitset>
#include <cstddef>


template <std::size_t N>
class BitFlagSimulator
{
public:
    explicit BitFlagSimulator(std::array<int, N> byte_maximums) : byte_maximums(std::move(byte_maximums)) {}
    int operator&(std::bitset<N> rhs);
    std::array<int, N> operator++(); // Prefix
    std::array<int, N> operator++(int); // Suffix

private:
    std::array<int, N> bytes{};
    std::array<int, N> byte_maximums;
};

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
    for (int i = N - 1; i >= 0; --i)
    {
        if (bytes[i] < byte_maximums[i])
        {
            ++bytes[i];
            break;
        }
    }

    return bytes;
}

template <std::size_t N>
std::array<int, N> BitFlagSimulator<N>::operator++(int)
{
    auto prev_bytes = bytes;

    for (int i = N - 1; i >= 0; --i)
    {
        if (bytes[i] < byte_maximums[i])
        {
            ++bytes[i];
            break;
        }
    }

    return prev_bytes;
}

#endif // BITFLAGSIMULATOR_H
