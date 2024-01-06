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

std::string MenuItem::getName() const {
    return this->name;
}

double MenuItem::getPrice() const {
    return this->price;
}

std::vector<class Ingredient> MenuItem::getIngredients() const {
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

// these 2 methods cannot call file.close() because MenuItem deserialization
//  will be called in a loop
void MenuItem::serialize(std::ofstream& file) const {
    if (!file.is_open()) return;
    
    double tempPrice = this->getPrice();
    size_t ingredientListLength = this->getIngredients().size();
    std::vector<class Ingredient> tempIngredients = this->getIngredients();

    file.write(reinterpret_cast<char*>(&serializedSignature), sizeof(SerializableTypes));
    serializeString(file, this->name);
    file.write(reinterpret_cast<char*>(&tempPrice), sizeof(tempPrice));
    file.write(reinterpret_cast<char*>(&ingredientListLength), sizeof(ingredientListLength));
    for (const auto& ing : tempIngredients) {
        ing.serialize(file);
    }
}

void MenuItem::deserialize(std::ifstream& file) {
    if (!file.is_open()) return;
    
    SerializableTypes typeFromFile;
    std::string tempName;
    double tempPrice;
    size_t tempIngredientsLength;
    std::vector<class Ingredient> tempIngredients;
    
    file.read(reinterpret_cast<char*>(&typeFromFile), sizeof(SerializableTypes));
    if (typeFromFile != SerializableTypes::MenuItem) return;
    
    tempName = deserializeString(file);
    file.read(reinterpret_cast<char*>(&tempPrice), sizeof(tempPrice));
    file.read(reinterpret_cast<char*>(&tempIngredientsLength), sizeof tempIngredientsLength);
        
    for (size_t i = 0; i < tempIngredientsLength; i++) {
        class Ingredient temp;
        temp.deserialize(file);
        std::cout << temp;
        tempIngredients.push_back(temp);
    }
    
    this->setName(tempName);
    this->setPrice(tempPrice);
    
    std::cout << "Before set: " << *this;
    
    this->setIngredients(tempIngredients);
    
    std::cout << "After set: " << *this;
}
