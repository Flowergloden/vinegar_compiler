//
// Created by huizeyang on 24-7-31.
//

#include <iostream>
#include <utils/ParsingTable.h>

int main()
{
    std::vector<std::string> raw{
        "test: A B C D",
    };

    // ParsingTable table(raw);
    // const auto node{std::make_shared<BNFNode>("root")};

    const BNFTree tree{"Test", "(AB CD) | {EF GH} [IJ]"};

    return 0;
}
