//
// Created by huizeyang on 24-8-6.
//

#ifndef BNFTREE_H
#define BNFTREE_H
#include <cassert>
#include <memory>
#include <string>
#include <utility>
#include <vector>


class BNFNode : public std::enable_shared_from_this<BNFNode>
{
public:
    BNFNode(std::string root, const std::shared_ptr<BNFNode> &parent_node) :
        root(std::move(root)), parent_node(parent_node)
    {
    }

    explicit BNFNode(std::string root) : root(std::move(root)) {}

    std::shared_ptr<BNFNode> add_node(const std::string &root)
    {
        nodes.emplace_back(std::make_shared<BNFNode>(root, shared_from_this()));
        return nodes.back();
    }

    std::shared_ptr<BNFNode> add_node(std::shared_ptr<BNFNode> target)
    {
        nodes.push_back(target);
        return target;
    }

    bool operator==(const BNFNode &rhs) const { return root == rhs.root; }
    bool operator==(const std::string &rhs) const { return root == rhs; }

    std::string root;

    std::weak_ptr<BNFNode> parent_node{};

    std::vector<std::shared_ptr<BNFNode>> nodes;
};

class BNFTree
{
public:
    explicit BNFTree(std::string non_terminal) : non_terminal(std::move(non_terminal)) {}
    BNFTree(std::string non_terminal, const std::string &pattern);

    inline static const std::string ROOT_NODE{"ROOT_NODE"};
    inline static const std::string OR_NODE{"OR_NODE"};
    inline static const std::string OPTIONAL_NODE{"OPTIONAL_NODE"};
    inline static const std::string GROUP_NODE{"GROUP_NODE"};
    inline static const std::string REPEAT_NODE{"REPEAT_NODE"};
    inline static const std::string EMPTY_NODE{"EMPTY_NODE"};

    [[nodiscard]] std::shared_ptr<BNFNode> get_root_node() const { return root_node; }

    const std::string non_terminal;

    bool operator==(const BNFTree &rhs) const { return non_terminal == rhs.non_terminal; }
    bool operator==(const std::string &rhs) const { return non_terminal == rhs; }

private:
    std::shared_ptr<BNFNode> root_node{std::make_shared<BNFNode>(ROOT_NODE)};
};


#endif // BNFTREE_H
