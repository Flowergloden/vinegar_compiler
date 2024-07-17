//
// Created by huizeyang on 24-7-16.
//

#ifndef DFA_H
#define DFA_H

// TEST ONLY
#include <iostream>

#include <set>
#include <vector>
#include <cassert>
#include "Token.h"

struct DFARaw
{
    TOKEN_TYPE type;
    std::string_view pattern;
};

class DFA
{
public:
    explicit DFA(const std::vector<DFARaw> &src);

    // TEST ONLY
    void test_dfa();

private:
    struct StateMoveUnit
    {
        int state;
        char cond;
        int next_state;
    };
    std::vector<StateMoveUnit> state_move_matrix;

    std::map<int, TOKEN_TYPE> final_state;

    const std::set<char> dfa_symbols{
        '[', ']', '-', '*', '+', '|',
    };
};


#endif // DFA_H
