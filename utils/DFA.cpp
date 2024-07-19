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
inline bool DFA::deal_with_symbols(int &state_now, const char prev_chr, const char chr, bool &has_or_syntax,
                                   bool &has_range_syntax, int &bracket, bool &just_match_bracket,
                                   bool &just_match_range_bracket,
                                   std::vector<std::vector<StateMoveUnit>> &state_buffer)
{
    if (dfa_symbols.contains(chr)) // TODO: impl bracket match
    {
        assert(!dfa_symbols.contains(prev_chr) && prev_chr != char{} && "Illegal DFA!!");
        std::vector<StateMoveUnit> &lateast_state_buffer{state_buffer[state_buffer.size() - 1]};

        switch (chr)
        {
        case '+':
            if (has_repeat_state_move_unit(state_now, prev_chr))
                return true;

            // in this case we add loop state movement for every range element
            if (just_match_range_bracket)
            {
                for (auto [state, cond, next_state] : lateast_state_buffer)
                {
                    state_move_matrix.push_back({next_state, cond, next_state});
                }
                just_match_range_bracket = false;
                return true;
            }

            // in this case we just add a state movement from end to start
            if (just_match_bracket)
            {
                state_move_matrix.push_back({lateast_state_buffer[lateast_state_buffer.size() - 1].next_state,
                                             lateast_state_buffer[0].cond, lateast_state_buffer[0].next_state});

                just_match_bracket = false;
                return true;
            }

            if (bracket)
            {
                lateast_state_buffer.push_back({state_now, prev_chr, state_now});
            }

            state_move_matrix.push_back({state_now, prev_chr, state_now});

            break;

        case '|':
            has_or_syntax = true; // just sign it
            break;
        case '-':
            has_range_syntax = true; // just sign it
            break;

        default:
            std::cerr << "Not impletmented DFA symbol: " << chr << std::endl;
            break;
        }

        return true;
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

        std::vector<std::vector<StateMoveUnit>> state_buffer{};
        for (const auto chr : raw_pattern)
        {
            if (chr == ' ')
                continue;

            // sign / unsign brackets
            if (chr == '[' || chr == '(')
            {
                state_buffer.emplace_back();
                ++bracket;
                continue;
            }

            if (chr == ']')
            {
                assert(bracket > 0 && "Unmatched bracket!!");
                --bracket;
                just_match_range_bracket = true;
                continue;
            }

            if (chr == ')')
            {
                assert(bracket > 0 && "Unmatched bracket!!");
                --bracket;
                just_match_bracket = true;
                continue;
            }

            if (deal_with_symbols(state_now, prev_chr, chr, has_or_syntax, has_range_syntax, bracket,
                                  just_match_bracket, just_match_range_bracket, state_buffer)) // has side-effect
                continue;

            // what actually deal with or syntax
            if (has_or_syntax)
            {
                auto [state, cond, next_state] = state_move_matrix[state_move_matrix.size() - 1];
                if (has_repeat_state_move_unit(state, chr))
                    continue;
                state_move_matrix.push_back({state, chr, next_state});

                if (bracket)
                {
                    state_buffer[state_buffer.size() - 1].push_back({state, chr, next_state});
                }

                if (just_match_bracket)
                {
                    // TODO: before this, need to impl multi-level bracket first
                }

                has_or_syntax = false;
                continue;
            }

            // TODO: deal with other syntax
            // what actually deal with range syntax
            if (has_range_syntax)
            {
                assert(bracket > 0 && prev_chr <= chr && "Illegal DFA range def!!");

                auto [state, cond, next_state] = state_move_matrix[state_move_matrix.size() - 1];
                state_move_matrix.pop_back();
                state_buffer.pop_back();
                for (char i = prev_chr; i <= chr; ++i)
                {
                    if (has_repeat_state_move_unit(state_now, i))
                        continue;
                    state_move_matrix.push_back({state, i, next_state});

                    // There must have been at least one bracket
                    state_buffer[state_buffer.size() - 1].push_back({state, i, next_state});

                    // There cant be a just_match_brachet
                }

                has_range_syntax = false;
                continue;
            }

            if (has_repeat_state_move_unit(state_now, chr))
                continue;

            state_move_matrix.push_back({state_now, chr, max_state});
            if (bracket)
            {
                state_buffer[state_buffer.size() - 1].push_back({state_now, chr, max_state});
            }
            state_now = max_state;
            ++max_state;
            prev_chr = chr;
            if (just_match_bracket || just_match_range_bracket)
            {
                just_match_bracket = false;
                just_match_range_bracket = false;
                state_buffer.pop_back();
            }
        }

        assert(bracket == 0 && "Unmatched bracket!!");
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
