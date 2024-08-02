//
// Created by huizeyang on 24-7-31.
//

#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include <memory>
#include <string>
#include <vector>
#include "Token.h"

class EBNFTree
{
public:
    class EBNFNode
    {
    public:
        explicit EBNFNode(std::string root) : root(std::move(root)){};

        std::shared_ptr<EBNFNode> add_node(const std::string &root)
        {
            return nodes.emplace_back(std::make_shared<EBNFNode>(root));
        }

        [[nodiscard]] auto &get_root() const { return root; }
        [[nodiscard]] auto &get_nodes() const { return nodes; }

    private:
        std::string root;
        std::vector<std::shared_ptr<EBNFNode>> nodes{};
    };

    explicit EBNFTree(std::string non_terminal, const std::string &pattern);

    std::string non_terminal;
    [[nodiscard]] std::shared_ptr<EBNFNode> get_root_node() const { return root; }

    const std::string root_element{"ROOT_ELEMENT"};
    const std::string or_symbol{"OR_SYMBOL"};
    const std::string optional_symbol{"OPTIONAL_SYMBOL"};
    const std::string group_symbol{"GROUP_SYMBOL"};

private:
    std::shared_ptr<EBNFNode> root{std::make_shared<EBNFNode>(root_element)};
};

class ParsingTable
{
public:
    explicit ParsingTable(const std::vector<std::string> &raw);

    [[nodiscard]] auto &get_first_set() const { return first_set; }

    [[nodiscard]] auto &get_follow_set() const { return follow_set; }

private:
    std::vector<EBNFTree> bnf_trees;
    std::map<std::string, std::vector<std::string>> first_set{};
    std::map<std::string, std::vector<std::string>> follow_set{};

    const std::string optional_terminal{"OPTIONAL"};

    void ebnf_2_bnf(const std::map<std::string, std::string> &ebnf);

    void calculate_first_set();
    void calculate_follow_set();
};


#endif // PARSING_TABLE_H
