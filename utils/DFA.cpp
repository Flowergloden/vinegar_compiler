//
// Created by huizeyang on 24-7-16.
//

#include "DFA.h"
DFA::DFA(const std::vector<DFARaw> &src)
{
    for (auto [type, pattern] : src)
    {
        const auto token_type = type;
        auto raw_pattern = pattern;

        int total_state{1};
        int state_now{0};
        for (const auto chr : raw_pattern)
        {
            if (chr == ' ')
                continue;

            bool has_same{false};
            for (auto [state, cond, next_state] : state_move_matrix)
            {
                if (state == state_now && cond == chr)
                {
                    has_same = true;
                    state_now = next_state;
                    break;
                }
            }

            if (has_same)
                continue;

            state_move_matrix.push_back({state_now, chr, total_state});
            state_now = total_state;
            ++total_state;
        }

        final_state[state_now] = token_type;
    }
}

// TEST ONLY
void DFA::test_dfa()
{
    std::cout << "state move matrix: \n";
    for (auto [state, cond, next_state] : state_move_matrix)
    {
        std::cout << state << "  =>  " << next_state << "  by  " << cond << std::endl;
    }

    std::cout << "final states: \n";
    for (const auto [state, token_type] : final_state)
    {
        std::cout << state << ": " << token_type << std::endl;
    }
}
