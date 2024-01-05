#pragma once
#include "ISerializable.h"
#include "globals.h"
#include <string>
#include <vector>

class Ingredient : ISerializable {
private:
    static SerializableTypes serializedSignature;
    std::string name = "Unknown";
    int quantity = 0;
public:
    Ingredient();
    Ingredient(std::string name, int quantity);
    friend std::ostream& operator<<(std::ostream& out, const Ingredient& i);
    void serialize(std::ofstream& file) const;
    void deserialize(std::ifstream& file);
    std::string getName() const;
    int getQuantity() const;
    void setName(std::string name);
    void setQuantity(int quantity);
};
