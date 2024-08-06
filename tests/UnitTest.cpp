//
// Created by huizeyang on 24-7-31.
//

#include <utils/ParsingTable.h>
#include <utils/BNFTree.h>

int main()
{
    std::vector<std::string> raw{
        "test: A B C D",
    };

    // ParsingTable table(raw);
    BNFNode node{"root"};

    node.add_node("node1");

    return 0;
}
