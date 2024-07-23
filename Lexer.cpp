//
// Created by huizeyang on 24-7-15.
//

#include "Lexer.h"
Lexer::Lexer(DFA &dfa, const std::string_view raw) : dfa(dfa)
{
    auto chr = raw.begin();
    const auto end = raw.end();
    while (chr < end)
    {
        if (this->dfa.separators.contains(*chr))
        {
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
            while (chr != end && *chr != '\n')
            {
                ++chr;
            }
        }
        else
        {
            tokens.push_back({type, lexeme});
        }
        ++chr;
        // TODO: deal with comment
    }
}
void Lexer::print_all_tokens()
{
    for (const auto &[token_type, lexeme, value, type] : tokens)
    {
        std::cout << "lexeme: " << lexeme << "; token type: " << token_type << "; value: " << value
                  << "; type: " << type << std::endl;
    }
}
