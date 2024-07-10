#include <iostream>
#include "parser_string.h"

namespace Parser {

std::string LevelToString(int level)
{
    std::string result;

    switch (level) {
        case 1:
            result = "Easy";
            break;
        case 2:
            result = "Medium";
            break;
        case 3:
            result = "Hard";
            break;
        default:
            result = "";
            break;
    }

    return result;
}

}