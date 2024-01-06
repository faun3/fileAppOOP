#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "globals.h"

class MenuItem {
private:
    static SerializableTypes serializedSignature;
    std::string name = "Unknown";
    double price = 0;
    std::vector<class Ingredient> ingredients;
public:
    MenuItem();
    MenuItem(std::string name, double price, std::vector<class Ingredient> ingredients);
    friend std::ostream& operator<<(std::ostream& out, const MenuItem& m);
    void serialize(std::ostream& file) const;
    void deserialize(std::istream& file);
    std::string getName();
    double getPrice();
    std::vector<class Ingredient> getIngredients();
    void setName(std::string newName);
    void setPrice(double newPrice);
    void setIngredients(std::vector<class Ingredient> newIngredients);
};
