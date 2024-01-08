#pragma once
#include <string>

class CheckExtension {
public:
    static bool checkExtension(const std::string& filename);
    static bool checkExtensionNoCsv(const std::string& filename);

};
