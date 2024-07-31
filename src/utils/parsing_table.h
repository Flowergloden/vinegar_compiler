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
    explicit ParsingTable(std::vector<std::string> &raw);

    [[nodiscard]] auto &get_first_set() const { return first_set; }

    [[nodiscard]] auto &get_follow_set() const { return follow_set; }

private:
    std::map<std::string, std::vector<std::string>> first_set{};
    std::map<std::string, std::vector<std::string>> follow_set{};

    std::map<std::string, std::vector<std::string>> bnf{};

    const std::string optional_terminal{"OPTIONAL"};

    void ebnf_2_bnf(std::map<std::string, std::string> ebnf);

    void calculate_first_set();
    void calculate_follow_set();
};


#endif // PARSING_TABLE_H
