#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "globals.h"
#include "ISerializable.h"

class MenuItem : ISerializable {
private:
    static SerializableTypes serializedSignature;
    std::string name = "Unknown";
    double price = 0;
    std::vector<class Ingredient> ingredients;
public:
    MenuItem();
    MenuItem(std::string name, double price, std::vector<class Ingredient> ingredients);
    friend std::ostream& operator<<(std::ostream& out, const MenuItem& m);
    void serialize(std::ofstream& file) const;
    void deserialize(std::ifstream& file);
    std::string getName() const;
    double getPrice() const;
    std::vector<class Ingredient> getIngredients() const;
    void setName(std::string newName);
    void setPrice(double newPrice);
    void setIngredients(std::vector<class Ingredient> newIngredients);
    bool operator<(const class MenuItem& rhs) const;
};
