#include "Logger.h"


std::string Logger::appendTime(std::string lineToLog) {
    // Get the current system time point
    auto currentTimePoint = std::chrono::system_clock::now();

    // Convert the time point to a time_t object
    std::time_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);

    // Convert the time_t to a local time struct
    std::tm* localTime = std::localtime(&currentTime);

    // Print the local time
    std::stringstream timestamp;
    timestamp << "[" << std::put_time(localTime, "%c") << "] ";
    
    return timestamp.str() + lineToLog + "\n";
}
