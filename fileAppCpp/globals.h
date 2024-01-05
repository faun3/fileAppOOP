#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

const std::vector<std::string> allowedExtensions{"txt", "csv", "bin"};

enum SerializableTypes {
    Ingredient,
    MenuItem,
    Stock,
    Menu,
    Order
};

// we need inline to avoid a linker error
// we can move the definition of the function to a .cpp file
// and keep the declaration here
// to avoid using "inline"
inline void serializeString(std::ofstream& file, std::string str) {
    if (!file.is_open()) return;
    size_t strLenght = str.size();
    // write the length of the string first
    file.write(reinterpret_cast<char*>(&strLenght), sizeof strLenght);
    // then its actual data
    file.write(str.data(), strLenght);
}

inline std::string deserializeString(std::ifstream& file) {
    std::string mt;
    if (!file.is_open()) return mt;
    size_t length;
    
    // get string length from the file
    file.read(reinterpret_cast<char*>(&length), sizeof(length));
    // resize with stdlib function (magic)
    mt.resize(length);
    // write into the character array
    file.read(&mt[0], length);
    return mt;
}

//template <typename T>
//inline std::vector<T> deserializeVector(std::ifstream& file) {
//    std::vector<T> result;
//    if (!file.is_open()) {
//        return result;
//    }
//    
//    std::size_t size;
//    file.read(reinterpret_cast<char*>(&result), sizeof(size));
//    
//    for (std::size_t i = 0; i < size; i++) {
//        T obj;
//        if (obj.deserialize(file)) {
//            result.push_back(obj);
//        }
//        else {
//            std::vector<T> empty;
//            file.close();
//            return empty;
//        }
//    }
//    file.close();
//    return result;
//}
