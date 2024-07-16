//
// Created by huizeyang on 24-7-15.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <map>
#include <string_view>


enum TOKEN_TYPE
{
    TOKEN_DEF_BEGIN,

    ID,
    COMMENT,
    LITERAL,

    // keywords
    EXTERN,

    // marks
    GREATER_THAN, // >
    QUESTION, // ?
    BLACK_TICK, // `
    CHARP, // #
    L_BRACKET, // {
    R_BRACKET, // }
    VERTICAL_BAR, // |
    ARROW, // =>

    EXTERN_TYPE,

    TOKEN_DEF_END,
};

struct Token
{
    TOKEN_TYPE token_type;
    std::string_view lexem;
    int num_value;
};

constexpr std::map<std::string_view, Token> TOKEN_TABLE{
    {"True", {LITERAL, "True", true}},
    {"False", {LITERAL, "True", false}},
};


#endif // TOKEN_H
