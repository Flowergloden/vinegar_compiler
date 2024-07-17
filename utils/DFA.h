//
// Created by huizeyang on 24-7-16.
//

#ifndef DFA_H
#define DFA_H

#include <vector>
#include "Token.h"

struct DFARaw
{
    TOKEN_TYPE type;
    std::string_view pattern;
};

class DFA
{
public:
    explicit DFA(const std::vector<DFARaw>& src);

private:
    struct StateMoveUnit
    {
        int state;
        char cond;
        int next_state;
    };
    std::vector<StateMoveUnit> state_move_matrix;

    std::map<int, TOKEN_TYPE> final_state;
};


#endif // DFA_H
