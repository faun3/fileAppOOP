#include "MenuItem.h"
#include "Ingredient.h"
#include <vector>
#include <string>
#include <iostream>

MenuItem::MenuItem(std::string name, double price, std::vector<class Ingredient> ingredients) {
    this->name = name;
    this->price = price;
    this->ingredients = ingredients;
}
std::ostream& operator<<(std::ostream& out, const class MenuItem& m) {
    out << "---Menu Item---\n";
    out << "Name: " << m.name << "\n";
    out << "Price: " << m.price << "\n";
    out << "Ingredients: \n";
    for (const auto& ing : m.ingredients) {
        out << ing;
    }
    return out;
}
