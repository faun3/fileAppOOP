#include "Ingredient.h"
#include <iostream>
#include <fstream>

Ingredient::Ingredient(std::string name, int quantity) {
    this->name = name;
    this->quantity = quantity;
}

std::ostream& operator<<(std::ostream& out, const class Ingredient& i) {
    out << "---Ingredient---\n";
    out << "Name: " << i.name << "\n";
    out << "Quantitiy: " << i.quantity << "\n";
    return out;
}

void Ingredient::serialize(std::ofstream& file) {
    // file.write();
}

void Ingredient::deserialize(std::ifstream& file) {
    
}
