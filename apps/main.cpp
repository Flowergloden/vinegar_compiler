#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include "../src/Lexer.h"
#include "../src/utils/DFA.h"
#include "../src/utils/Token.h"

int main(const int argc, char *argv[])
{
    assert(argc >= 3 && "missing parameter!!");

    std::ifstream dfa_def{argv[1]};

    assert(dfa_def.is_open() && "error when opening dfa defination file!!");

    std::vector<DFARaw> dfa_samples{};
    std::string raw{};
    while (getline(dfa_def, raw))
    {
        auto pos = raw.find(':');
        TOKEN_TYPE type = get_enum_by_name(raw.substr(0, pos));
        std::string src{raw.substr(pos + 1, raw.size() - pos)};
        dfa_samples.push_back({type, src});
    }
    dfa_def.close();

    const std::unique_ptr<DFA> dfa{new DFA(dfa_samples)};
    const std::unique_ptr<Lexer> lexer{new Lexer(*dfa)};

    dfa->test_dfa();

    std::string raw_code{};
    std::ifstream code_file{argv[2]};
    assert(code_file.is_open() && "error when opening code file!!");
    while (getline(code_file, raw_code))
    {
        lexer->add_tokens(raw_code);
    }
    code_file.close();

    lexer->print_all_tokens();

    return 0;
}
