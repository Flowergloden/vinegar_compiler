//
// Created by 13270 on 24-9-24.
//

#ifndef BITFLAGSIMULATOR_H
#define BITFLAGSIMULATOR_H
#include <array>
#include <bitset>
#include <cstddef>
#include <vector>


class BitFlagSimulator
{
public:
    explicit BitFlagSimulator(const std::vector<size_t> &byte_maximums) :
        length(byte_maximums.size()), byte_maximums(byte_maximums)
    {
        for (int i = 0; i < byte_maximums.size(); ++i)
        {
            bytes.push_back(0);
        }
    }

    const size_t length;
    static constexpr size_t MAX_LENGTH{16};

    int operator&(std::bitset<MAX_LENGTH> rhs) const;
    std::vector<int> operator++(); // Prefix
    std::vector<int> operator++(int); // Suffix

private:
    std::vector<int> bytes{};
    std::vector<size_t> byte_maximums{};
};


#endif // BITFLAGSIMULATOR_H
