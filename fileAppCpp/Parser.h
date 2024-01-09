#pragma once
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

class Parser {
public:
    static std::vector<std::string> splitCommand(std::string line);
};
