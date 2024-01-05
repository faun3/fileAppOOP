#include "Ingredient.h"
#include <iostream>

Ingredient::Ingredient(std::string name, int quantity) {
    this->name = name;
    this->quantity = quantity;
}

std::ostream& operator<<(std::ostream& out, const Ingredient& i) {
    out << "---Ingredient---\n";
    out << "Name: " << i.name << "\n";
    out << "Quantitiy: " << i.quantity << "\n";
    return out;
}
