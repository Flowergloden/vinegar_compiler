//
// Created by huizeyang on 24-7-31.
//

#include <iostream>
#include <utils/ParsingTable.h>

int main()
{
    const std::vector<std::string> raw{"test: A B C D", "Test: (AB CD) | {EF GH} [IJ]",
                                       "Combine: AB | CD | EF | GH | IJ", "Combine_group: ((((AB CD )EF)GH)IJ)KL "};


    BitFlagSimulator<8> bit_flag{{1, 2, 3, 4, 5, 6, 7, 8}};
    constexpr std::bitset<8> mask{1};
    std::cout << (bit_flag & mask) << std::endl;

    // ParsingTable table(raw);
    // const auto node{std::make_shared<BNFNode>("root")};

    // const BNFTree tree{"Test", "(AB CD) | {EF GH} [IJ]"};
    const ParsingTable table{raw};

    return 0;
}
