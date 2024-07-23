//
// Created by huizeyang on 24-7-15.
//

#include "Lexer.h"
Lexer::Lexer(DFA &dfa, const std::string_view raw) : dfa(dfa)
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
            std::cerr << "Unknow token: " << lexeme << std::endl;

        if (token_table.contains(lexeme))
        {
            tokens.push_back(token_table[lexeme]);
        }
        else
        {
            tokens.push_back({type, lexeme});
        }
        // TODO: deal with comment
    }
}
void Lexer::print_all_tokens()
{
    for (auto [token_type, lexeme, value, type] : tokens)
    {
        std::cout << "token type: " << token_type << "; lexeme: " << lexeme << "; value: " << value << "; type: " << type
                  << std::endl;
    }
}
