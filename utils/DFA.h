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
    std::string pattern;
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

    static bool has_duplicate_movement(int state, char cond, int next_state, std::vector<StateMoveUnit> &target)
    {
        for (auto [t_state, t_cond, t_next_state] : target)
        {
            if (state == t_state && cond == t_cond)
            {
                assert(next_state == t_next_state && "Unresolved condition!!"); // TODO: Deal with this
                return true;
            }
        }
        return false;
    }

    int move(int state, char cond);

    static void pre_process(std::string &raw)
    {
        for (auto i = raw.begin(); i != raw.end(); ++i)
        {
            if (*i == ' ')
            {
                raw.erase(i);
                --i;
            }
        }
    };

    static void buffer_passthrough(std::vector<std::vector<StateMoveUnit>> &state_buffer)
    {
        assert(state_buffer.size() >= 2 && "buffer failed!!");
        auto &latest_buffer = state_buffer.back();
        auto &second_latest_buffer = state_buffer[state_buffer.size() - 2];

        // second_latest_buffer.insert(second_latest_buffer.end(), latest_buffer.begin(), latest_buffer.end());
        for (auto [state, cond, next_state] : latest_buffer)
        {
            if (!has_duplicate_movement(state, cond, next_state, second_latest_buffer))
            {
                second_latest_buffer.push_back({state, cond, next_state});
            }
        }
    };

    static void buffer_passthrough(const std::vector<std::vector<StateMoveUnit>> &state_buffer,
                                   std::vector<StateMoveUnit> &state_move_matrix)
    {
        assert(state_buffer.size() == 1 && "Unmatched bracket!!");

        auto buffer = state_buffer.front();
        state_move_matrix.insert(state_move_matrix.end(), buffer.begin(), buffer.end());
    }
};


#endif // DFA_H
