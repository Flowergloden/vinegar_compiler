//
// Created by huizeyang on 24-7-16.
//

#ifndef DFA_H
#define DFA_H

// TEST ONLY
#include <iostream>

#include <cassert>
#include <set>
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
        '-',
        '*',
        '+',
        '|',
    };

    bool has_repeat_state_move_unit(int &state_now, std::string_view::value_type chr);
    bool deal_with_symbols(int &state_now, const char prev_chr, const char chr, bool &has_or_syntax,
                           bool &has_range_syntax, int &bracket, bool &just_match_bracket,
                           bool &just_match_range_bracket, std::vector<std::vector<StateMoveUnit>> &state_buffer);
};


#endif // DFA_H
