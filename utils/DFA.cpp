//
// Created by huizeyang on 24-7-16.
//

#include "DFA.h"

DFA::DFA(const std::vector<DFARaw> &src)
{
    int totol_state{0}; // how many states do we have now

    for (auto [token_type, raw] : src)
    {
        pre_process(raw);

        int state_now{0};
        int bracket{0};
        std::vector<std::vector<StateMoveUnit>> state_buffer{{}};

        // flags
        bool just_match_bracket{false};
        bool just_match_range_bracket{false};
        bool or_syntax_request_flag{false};
        bool or_syntax_is_waiting_bracket{false};
        bool or_syntax_need_keep_a_bracket{false};

        for (auto chr = raw.begin(); chr != raw.end(); ++chr)
        {
            auto &latest_state_buffer = state_buffer.back();
            switch (*chr)
            {
            case ' ':
                continue;
            case '(':
            case '[':
                ++bracket;
                state_buffer.emplace_back();
                break;
            case ')':
                --bracket;
                just_match_bracket = true;
                break;
            case ']':
                --bracket;
                just_match_range_bracket = true;
                break;
            case '+':
#pragma region +_SOLUTION
                // in this case we need to find out all movement that lead to ending state in this buffer
                // and all movement that start from origin state in this buffer
                // then add a loopback for each origin-ending pair
                if (just_match_bracket || just_match_range_bracket)
                {
                    const int origin_state{latest_state_buffer.front().state};
                    const int dest_state{state_now};

                    std::vector<StateMoveUnit> tmp_states{};
                    for (auto [d_state, d_cond, d_next_state] : latest_state_buffer)
                    {
                        if (d_next_state == dest_state)
                        {
                            for (auto [o_state, o_cond, o_next_state] : latest_state_buffer)
                            {
                                if (o_state == origin_state)
                                {
                                    tmp_states.push_back({d_next_state, o_cond, o_next_state});
                                }
                            }
                        }
                    }
                    latest_state_buffer.insert(latest_state_buffer.end(), tmp_states.begin(), tmp_states.end());
                }
                else // in this case we just need to add a loopback for previous movement
                {
                    const auto &[state, cond, next_state]{latest_state_buffer.back()};
                    latest_state_buffer.push_back({next_state, cond, next_state});
                }
#pragma endregion
                break;
            case '-':
#pragma region RANGE_SOLUTION
                if constexpr (true) // this external block targets at avoiding crosses initialization in switch jumping
                {
                    assert(chr - 1 != raw.begin() && !dfa_symbols.contains(*(chr - 1)) && "Unresolved symbol: \"-\"");
                    const int state{latest_state_buffer.back().state};
                    const int next_state{latest_state_buffer.back().next_state};
                    latest_state_buffer.pop_back(); // fall back previous movement
                    const char lhs{*(chr - 1)};
                    const char rhs{*(chr + 1)};
                    for (char i = lhs; i <= rhs; ++i)
                    {
                        latest_state_buffer.push_back({state, i, next_state});
                    }
                }
                ++chr;
#pragma endregion
                break;
            case '|':
#pragma region OR_SOLUTION
                assert(chr + 1 != raw.end() && !dfa_symbols.contains(*(chr + 1)) && "Unresolved symbol: \"|\"");
                if (*(chr + 1) == '(' || *(chr + 1) == '[')
                {
                    or_syntax_request_flag = true;

                    if (*(chr - 1) != ')' && *(chr - 1) != ']')
                    {
                        state_buffer.push_back({latest_state_buffer.back()});
                        just_match_bracket = true;
                    }

                    or_syntax_need_keep_a_bracket = true;
                }
                else
                {
                    if (*(chr - 1) == ')' || *(chr - 1) == ']')
                    {
                        const int state{latest_state_buffer.front().state};
                        const int next_state{latest_state_buffer.back().next_state};
                        latest_state_buffer.push_back({state, *(chr + 1), next_state});
                    }
                    else
                    {
                        latest_state_buffer.push_back(
                            {latest_state_buffer.back().state, *(chr + 1), latest_state_buffer.back().next_state});
                    }
                    ++chr;
                }
#pragma endregion
                break;

            default:
#pragma region DEFAULT_SOLUTION
                ++totol_state;

                if (int new_state{0}; has_duplicate_movement(new_state, *chr, latest_state_buffer))
                {
                    state_now = new_state;
                }
                else
                {
                    latest_state_buffer.push_back({state_now, *chr, totol_state});
                    state_now = totol_state;
                }
#pragma endregion
            }

            // pop latest buffer and passthrough elements to previous buffer
            if ((just_match_bracket || just_match_range_bracket) &&
                (chr + 1 == raw.end() || !dfa_symbols.contains(*(chr + 1))))
            {
#pragma region OR_SOLUTION_MULTIPLE
                if (or_syntax_is_waiting_bracket)
                {
                    auto &second_latest_state_buffer{state_buffer[state_buffer.size() - 2]};

                    const int o_state{second_latest_state_buffer.front().state};
                    const int d_state{second_latest_state_buffer.back().next_state};

                    latest_state_buffer.front().state = o_state;
                    latest_state_buffer.back().next_state = d_state;
                    state_now = d_state;
                    --totol_state;

                    buffer_passthrough(state_buffer);
                    state_buffer.pop_back();
                    or_syntax_is_waiting_bracket = false;
                }

                if (or_syntax_request_flag)
                {
                    or_syntax_is_waiting_bracket = true;
                    or_syntax_request_flag = false;
                }
#pragma endregion

                if (!or_syntax_need_keep_a_bracket)
                {
                    buffer_passthrough(state_buffer);
                    state_buffer.pop_back();
                    --bracket;
                }
                just_match_bracket = false;
                just_match_range_bracket = false;
                or_syntax_need_keep_a_bracket = false;
            }
        }
        assert(state_buffer.size() == 1 && "Unmatched bracket!!");

        final_state[state_now] = token_type;
        buffer_passthrough(state_buffer, state_move_matrix);
    }
}

int DFA::scan_move(std::string &lexeme, std::string_view::iterator &chr, const std::string_view::iterator &end)
{
    int state = first_state;

    while (true)
    {
        if (chr == end)
            return state;

        const int next_state = move(state, *chr);
        if (next_state == unknown_state)
            return state;

        state = next_state;
        lexeme.push_back(*chr);

        ++chr;
    }
}

int DFA::move(const int state, const char cond)
{
    for (auto [m_state, m_cond, m_next_state] : state_move_matrix)
    {
        if (m_state == state && m_cond == cond)
            return m_next_state;
    }
    return unknown_state;
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
