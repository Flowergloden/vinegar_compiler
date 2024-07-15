//
// Created by huizeyang on 24-7-15.
//

#ifndef TOKEN_H
#define TOKEN_H


constexpr enum TOKENS {
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


#endif // TOKEN_H
