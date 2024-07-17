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
    int total_state{1};
    for (auto [type, pattern] : src)
    {
        const auto token_type = type;
        auto raw_pattern = pattern;

        int state_now{0};
        char prev_chr{};
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
                    continue;

                case '*':
                    if (has_repeat_state_move_unit(state_now, prev_chr))
                        continue;
                    state_now = state_move_matrix[state_move_matrix.size() - 1].state;
                    state_move_matrix.pop_back();
                    state_move_matrix.push_back({state_now, prev_chr, state_now});
                    continue;

                case '|':
                    // TODO
                case '-':
                    // TODO

                default:
                    std::cerr << "Not impletmented DFA symbol: " << chr << std::endl;
                    break;
                }
            }

            prev_chr = chr;

            if (has_repeat_state_move_unit(state_now, chr))
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
