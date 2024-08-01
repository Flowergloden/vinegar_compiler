//
// Created by huizeyang on 24-7-31.
//

#include <utils/ParsingTable.h>

int main()
{
    std::vector<std::string> raw{
        "test: A B C D",
    };

    ParsingTable table(raw);

    return 0;
}
