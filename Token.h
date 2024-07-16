//
// Created by huizeyang on 24-7-15.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <map>
#include <string_view>


enum TOKENS
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

struct TokenValue
{
    TOKENS token_type;
    bool bool_value;
};

constexpr std::map<std::string_view, TokenValue> TOKEN_TABLE{
    {"True", {LITERAL, true}},
    {"False", {LITERAL, false}},
};


#endif // TOKEN_H
