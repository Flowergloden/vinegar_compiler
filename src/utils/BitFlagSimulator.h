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
    explicit BitFlagSimulator(std::array<int, N> byte_maximums);
    int operator&(std::bitset<N> rhs);
    std::array<int, N> operator++(); // Prefix
    std::array<int, N> operator++(int); // Suffix

private:
    std::array<int, N> bytes;
    std::array<int, N> byte_maximums;
};


#endif // BITFLAGSIMULATOR_H
