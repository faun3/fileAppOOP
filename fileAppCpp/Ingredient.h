#pragma once
#include "ISerializable.h"
#include "globals.h"
#include <string>
#include <vector>

class Ingredient : ISerializable {
private:
    const static SerializableTypes serializedSignature = SerializableTypes::Ingredient;
    std::string name = "Unknown";
    int quantity = 0;
public:
    Ingredient(std::string name, int quantity);
    friend std::ostream& operator<<(std::ostream& out, const Ingredient& i);
    void serialize(std::ofstream& file);
    void deserialize(std::ifstream& file);
};
