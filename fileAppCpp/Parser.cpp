#include "Parser.h"

std::vector<std::string> Parser::splitCommand(std::string line) {
    std::vector<std::string> tokenizedLine;
    
    std::string token;
    std::istringstream iss(line);
    while (iss >> std::quoted(token)) {
        tokenizedLine.push_back(token);
    }
    
    return tokenizedLine;
}
