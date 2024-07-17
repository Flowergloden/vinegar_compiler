#include <iostream>
#include <vector>

#include "utils/DFA.h"
#include "utils/Token.h"

int main()
{
    const std::vector<DFARaw> dfa_samples{
        {GREATER_THAN, ">"},
    };

    DFA dfa(dfa_samples);
    dfa.test_dfa();

    return 0;
}
