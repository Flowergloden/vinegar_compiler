#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "Lexer.h"
#include "utils/DFA.h"
#include "utils/Token.h"

int main(const int argc, char *argv[])
{
    assert(argc >= 2 && "missing parameter!!");

    std::ifstream file{argv[1]};

    assert(file.is_open() && "error when opening dfa defination file!!");

    std::vector<DFARaw> dfa_samples{};
    std::string raw{};
    while (getline(file, raw))
    {
        auto pos = raw.find(':');
        TOKEN_TYPE type = get_enum_by_name(raw.substr(0, pos));
        std::string src{raw.substr(pos + 1, raw.size() - pos)};
        dfa_samples.push_back({type, src});
    }


    const std::string raw_code{"True False abc //123"};

    const std::unique_ptr<DFA> dfa{new DFA(dfa_samples)};
    dfa->test_dfa();

    const std::unique_ptr<Lexer> lexer{new Lexer(*dfa, raw_code)};
    lexer->print_all_tokens();

    return 0;
}
