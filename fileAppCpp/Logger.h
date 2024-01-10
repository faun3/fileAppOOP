#pragma once
#include <string>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

class Logger {
public:
    static std::string appendTime(std::string lineToLog);
};
