//
// Created by huizeyang on 24-7-31.
//

#include "ParsingTable.h"
ParsingTable::ParsingTable(const std::vector<std::string> &raw)
{
    std::map<std::string, std::string> ebnf{};

    for (auto &str : raw)
    {
        const auto separator{str.find(':')};
        const std::string non_terminal{str.substr(0, separator)};
        const std::string pattern{str.substr(separator + 1, str.size())};

        ebnf[non_terminal] = pattern;
    }

    ebnf_2_bnf(ebnf);
    calculate_first_set();
    calculate_follow_set();
}
void ParsingTable::ebnf_2_bnf(const std::map<std::string, std::string>& ebnf)
{

}

void ParsingTable::calculate_first_set() {}
void ParsingTable::calculate_follow_set() {}
