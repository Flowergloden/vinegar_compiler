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

    expand();
}

void BNFTree::combine_same_terms(const std::shared_ptr<BNFNode> &target_node)
{
    for (auto i = target_node->nodes.begin(); i != target_node->nodes.end(); ++i)
    {
        if (**i == *target_node)
        {
            const auto pre_node = *i;
            i = target_node->nodes.erase(i);
            for (const auto &node : pre_node->nodes)
            {
                i = target_node->nodes.insert(i, node);
            }
        }
    }
}

void BNFTree::deal_with_or(const std::shared_ptr<BNFNode> &target_node)
{
    std::vector<std::shared_ptr<BNFNode>> or_nodes{};
}

void BNFTree::expand_iter(const std::shared_ptr<BNFNode> &target_node, const std::string &non_terminal)
{
    for (const auto &node : target_node->nodes)
    {
        expand_iter(node, non_terminal);
    }

    // deal with specific syntax first
    if (*target_node == OPTIONAL_NODE)
    {
        target_node->root = OR_NODE;
        auto pre_nodes = std::move(target_node->nodes);
        std::vector<std::shared_ptr<BNFNode>> new_nodes{{std::make_shared<BNFNode>(GROUP_NODE)},
                                                        {std::make_shared<BNFNode>(EMPTY_NODE)}};
        new_nodes.front()->nodes = std::move(pre_nodes);
        target_node->nodes = std::move(new_nodes);
    }

    if (*target_node == REPEAT_NODE)
    {
        target_node->root = OR_NODE;
        auto pre_nodes = std::move(target_node->nodes);
        std::vector<std::shared_ptr<BNFNode>> new_nodes{{std::make_shared<BNFNode>(GROUP_NODE)},
                                                        {std::make_shared<BNFNode>(EMPTY_NODE)}};

        new_nodes.front()->nodes = std::move(pre_nodes);
        new_nodes.front()->add_node(non_terminal);

        target_node->nodes = std::move(new_nodes);
    }

    // expand children nodes to this
    if (*target_node == GROUP_NODE)
    {
        deal_with_or(target_node);
    }
    combine_same_terms(target_node);
}

template <int N>
BitFlagSimulator<N>::BitFlagSimulator(std::array<int, N> byte_maximums) : byte_maximums(std::move(byte_maximums))
{
    bytes = new std::array<int, N>(0);
}
template <int N>
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
template <int N>
std::array<int, N> BitFlagSimulator<N>::operator++()
{
}
template <int N>
std::array<int, N> BitFlagSimulator<N>::operator++(int)
{
}
