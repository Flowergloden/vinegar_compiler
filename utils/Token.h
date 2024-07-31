//
// Created by huizeyang on 24-7-15.
//

#ifndef TOKEN_H
#define TOKEN_H
#include <array>
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
    SHARP, // #
    L_BRACKET, // {
    R_BRACKET, // }
    VERTICAL_BAR, // |
    ARROW, // =>

    EXTERN_TYPE,

    UNKNOWN_TOKEN,
    TOKEN_DEF_END,
};

constexpr std::array<std::string, 16> TOKEN_TYPE_NAME{
    "token_def_begin",

    "id",
    "comment",
    "literal",

    "extern",

    "greater_than",
    "question",
    "black_tick",
    "sharp",
    "l_bracket",
    "r_bracket",
    "vertical_bar",
    "arrow",

    "extern_type",

    "unknown_token",
    "token_def_end",
};

inline TOKEN_TYPE get_enum_by_name(const std::string_view name)
{
    for (int i = 0; i < TOKEN_DEF_END; ++i)
    {
        if (TOKEN_TYPE_NAME[i] == name)
        {
            return static_cast<TOKEN_TYPE>(i);
        }
    }

    return UNKNOWN_TOKEN;
}

enum TYPE
{
    NONE,
    FUNCTION,
    DIALOGUE,
};

struct Token
{
    TOKEN_TYPE token_type;
    const std::string lexeme;
    int num_value;
    TYPE type_value;
};

inline std::map<std::string_view, Token> token_table{
    {"True", {LITERAL, "True", true}},
    {"False", {LITERAL, "False", false}},
    {"fn", {EXTERN_TYPE, "fn", 0, FUNCTION}},
    {"extern", {EXTERN, "extern"}},
};


#endif // TOKEN_H
