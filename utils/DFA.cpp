//
// Created by huizeyang on 24-7-16.
//

#include "DFA.h"
bool DFA::has_repeat_state_move_unit(int &state_now, const std::string_view::value_type chr)
{
    for (auto [state, cond, next_state] : state_move_matrix)
    {
        if (state == state_now && cond == chr)
        {
            state_now = next_state;
            return true;
        }
    }
    return false;
}
DFA::DFA(const std::vector<DFARaw> &src)
{
    int max_state{1};
    for (auto [type, pattern] : src)
    {
        const auto token_type = type;
        auto raw_pattern = pattern;

        int state_now{0};
        char prev_chr{};
        bool has_or_syntax{false};
        bool has_range_syntax{false};
        for (const auto chr : raw_pattern)
        {
            if (chr == ' ')
                continue;

            if (dfa_symbols.contains(chr)) // TODO: impl bracket match
            {
                assert(!dfa_symbols.contains(prev_chr) && prev_chr != char{} && "Illegal DFA!!");

                switch (chr)
                {
                case '+':
                    if (has_repeat_state_move_unit(state_now, prev_chr))
                        continue;
                    state_move_matrix.push_back({state_now, prev_chr, state_now});
                    break;

                case '*':
                    if (has_repeat_state_move_unit(state_now, prev_chr))
                        continue;
                    // fall back what we added last iter
                    state_now = state_move_matrix[state_move_matrix.size() - 1].state;
                    state_move_matrix.pop_back();
                    state_move_matrix.push_back({state_now, prev_chr, state_now});
                    break;

                case '|':
                    has_or_syntax = true; // just sign it
                    break;
                case '-':
                    // TODO

                default:
                    std::cerr << "Not impletmented DFA symbol: " << chr << std::endl;
                    break;
                }

                continue;
            }

            prev_chr = chr;

            // what actually deal with or syntax
            if (has_or_syntax)
            {
                auto [state, cond, next_state] = state_move_matrix[state_move_matrix.size() - 1];
                if (has_repeat_state_move_unit(state, chr))
                    continue;
                state_move_matrix.push_back({state, chr, next_state});
                has_or_syntax = false;

                continue;
            }

            if (has_repeat_state_move_unit(state_now, chr))
                continue;

            state_move_matrix.push_back({state_now, chr, max_state});
            state_now = max_state;
            ++max_state;
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
