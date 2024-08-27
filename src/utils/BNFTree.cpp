//
// Created by huizeyang on 24-8-6.
//

#include "BNFTree.h"

BNFTree::BNFTree(std::string non_terminal, const std::string &pattern) : non_terminal(std::move(non_terminal))
{
    std::shared_ptr<BNFNode> node = root_node;
    std::string buffer;

    for (auto chr = pattern.begin(); chr != pattern.end(); ++chr)
    {
        switch (*chr)
        {
        case ' ':
            if (!buffer.empty())
            {
                node->add_node(buffer);
                buffer.clear();
            }
            break;

        case '|':
            if (*node != ROOT_NODE)
            {
                assert(!root_node->parent_node.expired() && "parent node incorrectly expired!!");
                const auto new_node = node->parent_node.lock()->add_node(OR_NODE);
                new_node->add_node(node);
                node = new_node;
            }
            else
            {
                const auto forward_node = node->nodes.back();
                node->nodes.pop_back();
                const auto new_node = node->add_node(OR_NODE);
                new_node->add_node(forward_node);
                node = new_node;
            }
            break;
        default:
            buffer.push_back(*chr);
        }
    }
    // ensure no buffered non-terminal left
    if (!buffer.empty())
    {
        node->add_node(buffer);
        buffer.clear();
    }
}
