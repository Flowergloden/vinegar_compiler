//
// Created by huizeyang on 24-8-6.
//

#ifndef BNFTREE_H
#define BNFTREE_H
#include <memory>
#include <string>
#include <utility>
#include <vector>


class BNFTree
{
};

class BNFNode
{
public:
    BNFNode(std::string &&root, std::shared_ptr<BNFNode> parent_node) :
        root(std::move(root)), parent_node(std::move(parent_node))
    {
    }

    explicit BNFNode(std::string &&root) : root(std::move(root)), parent_node(nullptr) {}

    std::shared_ptr<BNFNode> add_node(std::string &&root)
    {
        nodes.emplace_back(std::move(root), std::make_shared<BNFNode>(*self));
        return std::make_shared<BNFNode>(nodes.back());
    };

    std::shared_ptr<BNFNode> self{this};

    std::string root;

    std::shared_ptr<BNFNode> parent_node;

    std::vector<BNFNode> nodes;
};


#endif // BNFTREE_H
