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

    // keywords
    SELECT,
    UNION,
    DIFFERENCE,
    CARTESIAN_PRODUCT,
    NATURAL_JOIN,
    THETA_JOIN,
    INTERSECTION,
    DIVISION,
    RANGE,

    // marks
    L_BRACKET, // (
    R_BRACKET, // )

    // separators
    BLANK,
    TABLE,

    UNKNOWN_TOKEN,
    TOKEN_DEF_END,
};

constexpr std::array<std::string, TOKEN_DEF_END + 1> TOKEN_TYPE_NAME{
    "token_def_begin",

    "id",
    "comment",

    "select",
    "union",
    "difference",
    "cartesian_product",
    "natural_join",
    "theta_join",
    "intersection",
    "division",
    "range",

    "l_bracket",
    "r_bracket",

    "blank",
    "table",

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
    {"select", {SELECT, "select"}},
    {"union", {UNION, "union"}},
    {"difference", {DIFFERENCE, "difference"}},
    {"cartesian_product", {CARTESIAN_PRODUCT, "cartesian_product"}},
    {"natural_join", {NATURAL_JOIN, "natural_join"}},
    {"theta_join", {THETA_JOIN, "theta_join"}},
    {"intersection", {INTERSECTION, "intersection"}},
    {"division", {DIVISION, "division"}},
    {"range", {RANGE, "range"}},
};


#endif // TOKEN_H
