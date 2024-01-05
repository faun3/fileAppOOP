#pragma once
#include <string>
#include <vector>

const std::vector<std::string> allowedExtensions{"txt", "csv", "dat"};

enum SerializableTypes {
    Ingredient,
    MenuItem,
    Stock,
    Menu,
    Order
};
