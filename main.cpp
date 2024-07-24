#include <iostream>
#include <memory>
#include <vector>

#include "Lexer.h"
#include "utils/DFA.h"
#include "utils/Token.h"

int main()
{
    const std::vector<DFARaw> dfa_samples{
        {ID, "abc"}
    };

    const std::string raw_code{"True False abc //123"};

    const std::unique_ptr<DFA> dfa{new DFA(dfa_samples, 1)};
    dfa->test_dfa();

    const std::unique_ptr<Lexer> lexer{new Lexer(*dfa, raw_code)};
    lexer->print_all_tokens();

    return 0;
}
