//
// Created by huizeyang on 24-7-31.
//

#ifndef PARSING_TABLE_H
#define PARSING_TABLE_H

#include <string>
#include <vector>
#include "Token.h"

class ParsingTable
{
public:
    explicit ParsingTable(const std::vector<std::string> &raw);

    [[nodiscard]] auto &get_first_set() const { return first_set; }

    [[nodiscard]] auto &get_follow_set() const { return follow_set; }

private:
    std::map<std::string, std::vector<std::string>> first_set{};
    std::map<std::string, std::vector<std::string>> follow_set{};

    std::map<std::string, std::vector<std::string>> bnf{};

    const std::string optional_terminal{"OPTIONAL"};

    void ebnf_2_bnf(const std::map<std::string, std::string> &ebnf);

    void calculate_first_set();
    void calculate_follow_set();
};

class EBNFTree
{
public:
    class EBNFNode
    {
    public:
        explicit EBNFNode(std::string root) : root(std::move(root)){};

        void add_node(const std::string &root) { nodes.emplace_back(root); }

        [[nodiscard]] auto &get_root() const { return root; }
        [[nodiscard]] auto &get_nodes() const { return nodes; }

    private:
        std::string root;
        std::vector<EBNFNode> nodes{};
    };

    explicit EBNFTree(std::string non_terminal, std::string pattern) :
        non_terminal(std::move(non_terminal)), root(std::move(pattern)){};

    std::string non_terminal;
    [[nodiscard]] EBNFNode get_root_node() const { return root; }

private:
    EBNFNode root;
};


#endif // PARSING_TABLE_H
