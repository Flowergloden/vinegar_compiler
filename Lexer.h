//
// Created by huizeyang on 24-7-15.
//

#ifndef LEXER_H
#define LEXER_H
#include "utils/DFA.h"
#include "utils/Token.h"


class Lexer
{
public:
    explicit Lexer(DFA &dfa) : dfa(dfa) {}

    std::vector<Token> tokens{};

    void add_tokens(std::string_view raw);

    // DEBUG ONLY
    void print_all_tokens();

private:
    DFA &dfa;
};


#endif // LEXER_H
