#include "globals.h"
#include "CheckExtension.h"
#include <string>

bool CheckExtension::checkExtension(const std::string& filename) {
    
    size_t dotPosition = filename.find_last_of(".");
    
    if (dotPosition == std::string::npos) return false;
    std::string extension = filename.substr(dotPosition + 1);
    
    for (const auto& ext : allowedExtensions) {
        if (ext == extension) return true;
    }
    return false;
}

bool CheckExtension::checkExtensionNoCsv(const std::string& filename) {
    
    size_t dotPosition = filename.find_last_of(".");
    
    if (dotPosition == std::string::npos) return false;
    std::string extension = filename.substr(dotPosition + 1);
    
    if (extension == "csv") return false;
    
    for (const auto& ext : allowedExtensions) {
        if (ext == extension) return true;
    }
    return false;
}

std::string CheckExtension::getExtension(const std::string& filename) {
    size_t dotPosition = filename.find_last_of(".");
    
    if (dotPosition == std::string::npos) return "";
    std::string extension = filename.substr(dotPosition + 1);
    
    return extension;
}
