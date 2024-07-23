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
    explicit Lexer(DFA& dfa, std::string_view raw);

    std::vector<Token> tokens{};

    // DEBUG ONLY
    void print_all_tokens();

private:
    DFA& dfa;
};


#endif // LEXER_H
