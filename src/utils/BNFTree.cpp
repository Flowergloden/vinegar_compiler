//
// Created by huizeyang on 24-8-6.
//

#include "BNFTree.h"

BNFTree::BNFTree(std::string non_terminal, const std::string &pattern) : non_terminal(std::move(non_terminal))
{
    std::shared_ptr<BNFNode> node = root_node;
    std::string buffer;

    for (const char chr : pattern)
    {
        switch (chr)
        {
        case ' ':
            if (!buffer.empty())
            {
                node->add_node(buffer);
                buffer.clear();
            }
            break;

        case '|':
            {
                const auto forward_node = node->nodes.back();
                node->nodes.pop_back();
                const auto new_node = node->add_node(OR_NODE);
                new_node->add_node(forward_node);
                node = new_node;
            }
            break;

        case '(':
            {
                const auto new_node = node->add_node(GROUP_NODE);
                node = new_node;
            }
            break;

        case ')':
        case ']':
        case '}':
            assert(*node != ROOT_NODE && "Unexpected BNF tree structure!!");
            assert(!node->parent_node.expired() && "parent node incorrectly expired!!");
            // ensure no buffered non-terminal left
            if (!buffer.empty())
            {
                node->add_node(buffer);
                buffer.clear();
            }

            node = node->parent_node.lock();
            break;

        case '[':
            {
                const auto new_node = node->add_node(OPTIONAL_NODE);
                node = new_node;
            }
            break;

        case '{':
            {
                const auto new_node = node->add_node(REPEAT_NODE);
                node = new_node;
            }
            break;

        default:
            buffer.push_back(chr);
        }

        // Check if or syntax has been fed with both lhs && rhs
        if (*node == OR_NODE && node->nodes.size() >= 2)
        {
            assert(!node->parent_node.expired() && "parent node incorrectly expired!!");
            node = node->parent_node.lock();
        }
    }
    // ensure no buffered non-terminal left
    if (!buffer.empty())
    {
        node->add_node(buffer);
        buffer.clear();
    }

    root_node->root = GROUP_NODE;
}
