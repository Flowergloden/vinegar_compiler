//
// Created by huizeyang on 24-7-31.
//

#include <iostream>
#include <utils/BNFTree.h>
#include <utils/ParsingTable.h>

int main()
{
    std::vector<std::string> raw{
        "test: A B C D",
    };

    // ParsingTable table(raw);
    const auto node{std::make_shared<BNFNode>("root")};

    node->add_node("node1");

    std::cout << (*node->nodes.front() == "node1") << std::endl;

    return 0;
}
