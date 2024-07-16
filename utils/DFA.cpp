//
// Created by huizeyang on 24-7-16.
//

#include "DFA.h"
DFA::DFA(const std::vector<DFARaw> &src)
{
    for (auto item : src)
    {
        auto name = item.name;
        auto raw_pattern = item.pattern;

        int total_state{1};
        int state_now{0};
        for (auto chr : raw_pattern)
        {
            if (chr == ' ') continue;

            bool has_same{false};
            for (auto unit : state_move_matrix)
            {
                if (unit.state == state_now && unit.cond == chr)
                {
                    has_same = true;
                    state_now = unit.next_state;
                    break;
                }
            }

            if (has_same) continue;

            state_move_matrix.push_back({state_now, chr, total_state});
            ++total_state;
        }
    }
}
