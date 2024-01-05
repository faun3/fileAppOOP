#pragma once
#include <string>
#include <vector>

class MenuItem {
private:
    std::string name = "Unknown";
    double price = 0;
    std::vector<class Ingredient> ingredients;
public:
    MenuItem(std::string name, double price, std::vector<class Ingredient> ingredients);
    friend std::ostream& operator<<(std::ostream& out, const MenuItem& m);
};
