#pragma once
#include <fstream>

class ISerializable {
public:
    virtual void serialize(std::ofstream& file) = 0;
    virtual void deserialize(std::ifstream& file) = 0;
};

