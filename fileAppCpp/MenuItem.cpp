#include "MenuItem.h"
#include "Ingredient.h"
#include "globals.h"
#include <vector>
#include <string>
#include <iostream>

SerializableTypes MenuItem::serializedSignature = SerializableTypes::MenuItem;

MenuItem::MenuItem() {
    this->name = "Unknown";
    this->price = 0;
    std::vector<class Ingredient> empty;
    this->ingredients = empty;
}

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

std::string MenuItem::getName() {
    return this->name;
}
double MenuItem::getPrice() {
    return this->price;
}
std::vector<class Ingredient> MenuItem::getIngredients() {
    return this->ingredients;
}
void MenuItem::setName(std::string newName) {
    if (newName != "") this->name = newName;
}
void MenuItem::setPrice(double newPrice) {
    if (newPrice >= 0) this->price = newPrice;
}
void MenuItem::setIngredients(std::vector<class Ingredient> newIngredients) {
    if (newIngredients.size() > 0) {
        this->ingredients = newIngredients;
    }
}
