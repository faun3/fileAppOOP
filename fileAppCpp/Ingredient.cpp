#include "Ingredient.h"
#include "globals.h"
#include <iostream>
#include <fstream>

SerializableTypes Ingredient::serializedSignature = SerializableTypes::Ingredient;

Ingredient::Ingredient() {
    this->name = "Unknown";
    this->quantity = 0;
}

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

std::string Ingredient::getName() const {
    return this->name;
}

int Ingredient::getQuantity() const {
    return this->quantity;
}

void Ingredient::setName(std::string name) {
    if (name != "") this->name = name;
}

void Ingredient::setQuantity(int quantity) {
    if (quantity > 0) this->quantity = quantity;
}

// this method cannot call file.close() because it will prevent proper serialization
//  when called in a loop (such as in MenuItem::serialize)
void Ingredient::serialize(std::ofstream& file) const {
    if (!file.is_open()) return;
    
    // this is pretty horrible afaik because you can modify the object
    // using the pointer you get from reinterpret_cast
    file.write(reinterpret_cast<const char*>(&Ingredient::serializedSignature), sizeof(SerializableTypes));
    serializeString(file, this->getName());
    
    int tempQty = this->getQuantity();
    
    file.write(reinterpret_cast<char*>(&tempQty), sizeof(tempQty));
}

// this method cannot call file.close() for the same reasons that serialize cannot call file.close()
void Ingredient::deserialize(std::ifstream& file) {
    if (!file.is_open()) {
        return;
    }
    
    // read the serializedSignature
    // this needs to be refactored into its own thing
    // read in an enum member, determine its value then call that class' deserialize method
    SerializableTypes readSignature;
    std::string tempName;
    int tempQuantity;

    file.read(reinterpret_cast<char*>(&readSignature), sizeof(SerializableTypes));
    if (readSignature != Ingredient::serializedSignature) return;
    
    // idk what to do here, mutate this with the read info?
    // probably the only thing you can do
    tempName = deserializeString(file);
    std::cout << "From file: " << tempName << "\n";
    this->setName(tempName);
    
    file.read(reinterpret_cast<char*>(&tempQuantity), sizeof(tempQuantity));
    this->setQuantity(tempQuantity);
}
