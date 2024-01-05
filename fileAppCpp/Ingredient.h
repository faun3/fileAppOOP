#pragma once
#include <string>
#include <vector>

class Ingredient {
private:
    std::string name = "Unknown";
    int quantity = 0;
public:
    Ingredient(std::string name, int quantity);
    friend std::ostream& operator<<(std::ostream& out, const Ingredient& i);
};
