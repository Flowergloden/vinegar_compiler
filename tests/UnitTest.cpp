//
// Created by huizeyang on 24-7-31.
//

#include <iostream>
#include <utils/ParsingTable.h>

int main()
{
    std::vector<std::string> raw{
        "test: A B C D",
        "Test: (AB CD) | {EF GH} [IJ]",
        "Combine: AB | CD | EF | GH | IJ",
        "Combine_group: ((((AB CD )EF)GH)IJ)KL "
    };

    // ParsingTable table(raw);
    // const auto node{std::make_shared<BNFNode>("root")};

    // const BNFTree tree{"Test", "(AB CD) | {EF GH} [IJ]"};
    const ParsingTable table{raw};

    return 0;
}
