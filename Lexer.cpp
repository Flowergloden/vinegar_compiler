//
// Created by huizeyang on 24-7-15.
//

#include "Lexer.h"
void Lexer::add_tokens(std::string_view raw)
{
    auto chr = raw.begin();
    const auto end = raw.end();
    while (chr != end)
    {
        if (this->dfa.separators.contains(*chr))
        {
            ++chr;
            continue;
        }

        std::string lexeme{};
        const int final_state{this->dfa.scan_move(lexeme, chr, end)};

        const TOKEN_TYPE type{this->dfa.get_token_type(final_state)};
        if (type == UNKNOWN_TOKEN)
            std::cerr << "Unknow token: " << lexeme << *chr << std::endl;

        if (token_table.contains(lexeme))
        {
            tokens.push_back(token_table[lexeme]);
        }
        else if (type == COMMENT)
        {
            while (chr + 1 != end && *chr != '\n')
            {
                ++chr;
            }
        }
        else
        {
            tokens.push_back({type, lexeme});
        }
        ++chr;
    }
}

void Lexer::print_all_tokens()
{
    std::cout << "\nTokens: \n";
    for (const auto &[token_type, lexeme, value, type] : tokens)
    {
        std::cout << "lexeme: " << lexeme << "; token type: " << TOKEN_TYPE_NAME[token_type] << "; value: " << value
                  << "; type: " << type << std::endl;
    }
}
