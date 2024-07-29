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

// TODO: think twice how to deal with repeat movements
bool DFA::has_repeat_state_move_unit(int &out_state_now, const std::string_view::value_type chr,
                                     const std::vector<StateMoveUnit> &src)
{
    for (auto [state, cond, next_state] : src)
    {
        if (state == out_state_now && cond == chr)
        {
            out_state_now = next_state;
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

        // flags
        int state_now{0}; // what's the state now
        char prev_chr{}; // the char in last iter
        bool has_or_syntax{false}; // there's an '|' syntax need to be prosseced
        bool has_range_syntax{false}; // there's an '[x-x]' syntax need to be prosseced
        int bracket{0}; // how many levels of bracket there is
        bool just_match_bracket{false}; // just ended a bracket-match last iter
        bool just_match_range_bracket{false}; // just ended a bracket-match for '[x-x]' syntax last iter
        bool or_syntax_is_waiting{false}; // '|' syntax is waiting for another element
        bool or_syntax_waiting_is_bracket{false}; // the element '|' syntax is waiting for is in a bracket
        bool or_syntax_need_keep_a_buffer{false}; // '|' syntax need to keep one more buffer to cache lhs
        std::vector<std::vector<StateMoveUnit>> state_buffer{};

        for (const auto chr : raw_pattern) // TODO: deal with multi-level bracket buffer
        {
            // pre-process
            if (chr == ' ')
                goto skip_whole_iter;

            if (chr == '[' || chr == '(')
            {
                // this means last or syntax has a bracket rhs
                // we need to do what we need to do to deal with or syntax with bracket lhs here (see about line 132)
                // because we will skip flag update this iter and leaving the or syntax flag to next iter is disaster
                if (has_or_syntax)
                {
                    or_syntax_is_waiting = true;
                    or_syntax_need_keep_a_buffer = true;
                    or_syntax_waiting_is_bracket = true;
                    has_or_syntax = false;
                    just_match_bracket = false;
                    just_match_range_bracket = false;
                }
                state_buffer.emplace_back();
                ++bracket;
                goto skip_evaluation_and_flag_update;
            }

            if (chr == ']')
            {
                assert(bracket > 0 && "Unmatched bracket!!");
                just_match_range_bracket = true;
                goto skip_evaluation_and_flag_update;
            }

            if (chr == ')')
            {
                assert(bracket > 0 && "Unmatched bracket!!");
                just_match_bracket = true;
                goto skip_evaluation_and_flag_update;
            }

            if (dfa_symbols.contains(chr))
            {
                // assert(!dfa_symbols.contains(prev_chr) && prev_chr != char{} && "Illegal DFA!!");
                std::vector<StateMoveUnit> &lateast_state_buffer{state_buffer.back()};

                switch (chr)
                {
                case '+':
                    if (has_repeat_state_move_unit(state_now, prev_chr))
                        goto skip_evaluation;

                    // in this case we add loop state movement for every range element
                    if (just_match_range_bracket)
                    {
                        for (auto [state, cond, next_state] : lateast_state_buffer)
                        {
                            state_move_matrix.push_back({next_state, cond, next_state});
                        }
                        goto skip_evaluation;
                    }

                    // in this case we just add a state movement from end to start
                    if (just_match_bracket)
                    {
                        for (auto [state, cond, next_state] : lateast_state_buffer)
                        {
                            if (next_state == state_now)
                            {
                                for (auto [l_state, l_cond, l_next_state] : lateast_state_buffer)
                                {
                                    if (l_next_state != lateast_state_buffer[0].next_state)
                                        break;

                                    if (has_repeat_state_move_unit(state_now, l_cond))
                                        continue;

                                    state_move_matrix.push_back(
                                        {state_now, l_cond, lateast_state_buffer[0].next_state});

                                    lateast_state_buffer.push_back(state_move_matrix.back());
                                }
                            }
                        }
                        goto skip_evaluation;
                    }

                    if (bracket)
                    {
                        lateast_state_buffer.push_back({state_now, prev_chr, state_now});
                    }

                    state_move_matrix.push_back({state_now, prev_chr, state_now});

                    goto skip_evaluation;

                case '|':
                    has_or_syntax = true; // just sign it
                    goto skip_evaluation_and_flag_update;
                case '-':
                    has_range_syntax = true; // just sign it
                    goto skip_evaluation_and_flag_update;

                default:
                    assert(false && "Not implemented dfa symbol!!");
                }
            }

            // what actually deal with or syntax
            if (has_or_syntax)
            {
                if (just_match_bracket)
                {
                    or_syntax_is_waiting = true;
                    or_syntax_need_keep_a_buffer = true;
                    goto skip_evaluation;
                }

                auto [state, cond, next_state] = state_move_matrix.back();
                if (has_repeat_state_move_unit(state, chr))
                    goto skip_evaluation;
                state_move_matrix.push_back({state, chr, next_state});

                if (bracket)
                {
                    state_buffer.back().push_back({state, chr, next_state});
                }

                goto skip_evaluation;
            }

            // what actually deal with range syntax
            if (has_range_syntax)
            {
                assert(bracket > 0 && prev_chr <= chr && "Illegal DFA range def!!");

                auto [state, cond, next_state] = state_move_matrix[state_move_matrix.size() - 1];
                state_move_matrix.pop_back();
                state_buffer.back().clear();
                for (char i = prev_chr; i <= chr; ++i)
                {
                    if (has_repeat_state_move_unit(state_now, i))
                        continue;
                    state_move_matrix.push_back({state, i, next_state});

                    // There must have been at least one bracket
                    state_buffer.back().push_back({state, i, next_state});

                    // There cant be a just_match_brachet
                }

                goto skip_evaluation;
            }

            // logic that deal with default case
            if (has_repeat_state_move_unit(state_now, chr))
                goto skip_evaluation;

            state_move_matrix.push_back({state_now, chr, max_state});
            if (bracket)
            {
                state_buffer[state_buffer.size() - 1].push_back({state_now, chr, max_state});
            }
            state_now = max_state;
            ++max_state;

            // what has to be done before skipping this iter
        skip_evaluation:
            if (just_match_bracket || just_match_range_bracket)
            {
                just_match_bracket = false;
                just_match_range_bracket = false;
                if (!or_syntax_need_keep_a_buffer)
                {
                    --bracket;
                    if (bracket)
                    {
                        auto &latest_buffer = state_buffer.back();
                        auto &second_latest_buffer = state_buffer[state_buffer.size() - 2];

                        second_latest_buffer.insert(second_latest_buffer.end(), latest_buffer.begin(),
                                                    latest_buffer.end());
                    }
                    state_buffer.pop_back();
                }
                else
                {
                    or_syntax_need_keep_a_buffer = false;
                }
            }
            has_or_syntax = false;
            has_range_syntax = false;
            prev_chr = chr;

        skip_evaluation_and_flag_update:
            if (or_syntax_is_waiting)
            {
                // in this case rhs is single, we just need to add a new state
                if (!or_syntax_waiting_is_bracket)
                {
                    assert(!dfa_symbols.contains(chr) && "Illegal DFA or syntax!!");

                    auto &latest_state_buffer = state_buffer.back();
                    state_move_matrix.push_back(
                        {latest_state_buffer[0].state, chr, latest_state_buffer.back().next_state});

                    state_buffer.pop_back();
                    or_syntax_is_waiting = false;
                }

                // in this case rhs is a bracket, we need to modify state and end of this part of state movements
                if (just_match_bracket)
                {
                    // NOTE: we do not support or syntax with single lhs and bracket rhs
                    assert(state_buffer.size() >= 2 &&
                           "state buffer missed somewhere!!(or the or syntax has a single lhs)");

                    auto &latest_state_buffer = state_buffer.back();
                    auto &second_latest_state_buffer = state_buffer[state_buffer.size() - 2];

                    int offset{0};
                    for (int i = 0; i < std::min(latest_state_buffer.size(), second_latest_state_buffer.size()); ++i)
                    {
                        if (latest_state_buffer[i].cond != second_latest_state_buffer[i].cond)
                            break;

                        ++offset;
                    }

                    auto s{state_move_matrix.begin()};
                    std::advance(s, state_move_matrix.size() - latest_state_buffer.size());
                    auto e{s};
                    std::advance(e, offset);
                    state_move_matrix.erase(s, e);

                    state_move_matrix[state_move_matrix.size() - latest_state_buffer.size() + offset].state =
                        second_latest_state_buffer[offset].state;

                    state_move_matrix.back().next_state = second_latest_state_buffer.back().next_state;
                    state_now = second_latest_state_buffer.back().next_state;
                    --max_state;
                }

                // the same as above
                if (just_match_range_bracket)
                {
                    assert(state_buffer.size() >= 2 &&
                           "state buffer missed somewhere!!(or the or syntax has a single lhs)");

                    auto &latest_state_buffer = state_buffer.back();
                    auto &second_latest_state_buffer = state_buffer[state_buffer.size() - 2];

                    auto unit{state_move_matrix.rbegin()};
                    auto end{state_move_matrix.rbegin()};
                    std::advance(end, latest_state_buffer.size());
                    for (; unit != end; ++unit)
                    {
                        unit->state = second_latest_state_buffer[0].state;
                        unit->next_state = second_latest_state_buffer.back().next_state;
                    }

                    state_now = second_latest_state_buffer.back().next_state;
                    --max_state;
                }
            }

        skip_whole_iter:
            continue;
        }

        // assert(bracket == 0 && "Unmatched bracket!!");
        final_state[state_now] = token_type;
    }
}

DFA::DFA(const std::vector<DFARaw> &src, int)
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
        bool or_syntax_is_waiting_bracket{false};
        bool or_syntax_need_keep_a_bracket{false};

        for (auto chr = raw.begin(); chr != raw.end(); ++chr)
        {
            auto &latest_state_buffer = state_buffer.back();
            switch (*chr)
            {
                // TODO: deal with symbols
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
                    // TODO: deal with dupicatation somewhere
                }
                else // in this case we just need to add a loopback for previous movement
                {
                    const auto &[state, cond, next_state]{latest_state_buffer.back()};
                    latest_state_buffer.push_back({next_state, cond, next_state});
                }
#pragma endregion
                break;
            case '-':
                { // this external block targets at avoiding crosses initialization in switch jumping
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
                break;
            case '|':
                assert(chr + 1 != raw.end() && !dfa_symbols.contains(*(chr + 1)) &&
                       "Unresolved symbol: \"|\"");
                if (*(chr + 1) == '(' || *(chr + 1) == '[')
                {
                    or_syntax_is_waiting_bracket = true;

                    if (*(chr - 1) == ')' || *(chr - 1) == ']')
                    {
                        or_syntax_need_keep_a_bracket = true;
                    }
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
                break;

            default:
#pragma region DEFAULT_SOLUTION
                ++totol_state;

                if (int new_state{0}; has_repeat_state_move_unit(new_state, *chr, latest_state_buffer))
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
                buffer_passthrough(state_buffer);
                state_buffer.pop_back();
                --bracket;
                just_match_bracket = false;
                just_match_range_bracket = false;
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
