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

    explicit DFA(const std::vector<DFARaw> &src, int);

    const int first_state{0};

    int scan_move(std::string &lexeme, std::string_view::iterator &chr, const std::string_view::iterator &end);

    TOKEN_TYPE get_token_type(const int state)
    {
        if (final_state.contains(state))
        {
            return final_state[state];
        }

        return UNKNOWN_TOKEN;
    }

    const std::set<char> separators{
        ' ',
        '\n',
        '\t',
    };

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

    const int unknown_state{0};

    bool has_repeat_state_move_unit(int &state_now, std::string_view::value_type chr);
    static bool has_repeat_state_move_unit(int &out_state_now, std::string_view::value_type chr,
                                    const std::vector<StateMoveUnit> &src);

    int move(int state, char cond);
};


#endif // DFA_H
