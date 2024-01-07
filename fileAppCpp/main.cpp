#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <sstream>

#include "globals.h"
#include "CheckExtension.h"
#include "ISerializable.h"
#include "Ingredient.h"
#include "MenuItem.h"
#include "Restaurant.h"

// how to get files from the directory where the main.cpp file is (aka sane people mode)
    // Product > Scheme > Edit Scheme
    // In the "Run" (in the left column) select "Options"
    // Under "Working Directory"
        // tick "Use custom Working Directory"
        // pick the project directory where the main.cpp file is as your working directory

// how to see the executable that results from a build
    // Product > Show Build Folder in Finder
    // Drag the build folder into your XCode File Explorer
        // The executable is usually under Products/Debug/<here>

// how to add command line arguments to your build and run
    // Product > Scheme > Edit Scheme
    // In the "Run" section select "Arguments"
        // You can now add, remove and reorder arguments that are passed before running the executable

class ITextReadableIngredient {
public:
    virtual std::vector<class Ingredient> readText(const std::string& filename) = 0;
    virtual ~ITextReadableIngredient() {}
};

class IngredientParser : ITextReadableIngredient {
public:
    std::vector<class Ingredient> readText(const std::string& filename) override {
        std::vector<class Ingredient> parsedIngredientList;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "File mishap" << std::endl;
            return parsedIngredientList;
        }
        std::string parsedName;
        int parsedQuantity;
        
        while (file >> parsedName >> parsedQuantity) {
            class Ingredient toPush(parsedName, parsedQuantity);
            parsedIngredientList.push_back(toPush);
        }
        
        file.close();
        return parsedIngredientList;
    }
};

class ITextReadableMenuItem {
public:
    virtual std::vector<class MenuItem> parseTextFile(const std::string& filename) = 0;
    virtual ~ITextReadableMenuItem() {}
};

class MenuItemParser : ITextReadableMenuItem {
public:
    std::vector<class MenuItem> parseTextFile(const std::string& filename) override {
        std::vector<class MenuItem> result;
        
        std::ifstream file(filename);
        if (!file.is_open()) return result;
        
        std::string menuItemName;
        double menuItemPrice;
        int ingredientListLength;
        std::vector<class Ingredient> ingredients;
        std::string ingredientName;
        int ingredientQuantity;
        
        while (file >> menuItemName >> menuItemPrice >> ingredientListLength) {
            for (int step = 0; step < ingredientListLength; step++) {
                file >> ingredientName >> ingredientQuantity;
                class Ingredient toPush(ingredientName, ingredientQuantity);
                ingredients.push_back(toPush);
            }
            class MenuItem parsedMenuItem(menuItemName, menuItemPrice, ingredients);
            result.push_back(parsedMenuItem);
            ingredients.clear();
        }
        
        return result;
    }
};


int main(int argc, const char * argv[]) {
    if (argc != 3) {
        std::cout << "No data files were specified. Using default data.\n";
    }
    else {
        
    }
    
    IngredientParser ip;
    std::vector<class Ingredient> parsed;
    parsed = ip.readText(argv[1]);
    for (const auto& ing : parsed) {
        std::cout << ing << "\n";
    }
    
    MenuItemParser mip;
    std::vector<class MenuItem> parsedVector;
    parsedVector = mip.parseTextFile(argv[2]);
    
    class MenuItem pizza = parsedVector.at(0);
    class MenuItem empty;
        
    bool reading = true;
    
    if (reading) {
        std::ifstream file("menuItem.bin", std::ios::binary);
        if (!file.is_open()) std::cout << "File didn't open.\n";
        empty.deserialize(file);
        std::cout << "Deserialized: \n" << empty;
        file.close();
    }
    else {
        std::ofstream file("menuItem.bin", std::ios::binary);
        if (!file.is_open()) std::cout << "File didn't open.\n";
        pizza.serialize(file);
        std::cout << "Serialized: \n" << pizza;
        file.close();
    }
    
    // this completely solves the quoted string problem for free
    // just need to add a parser class to make it work
//    std::string userInput;
//    std::vector<std::string> tokens;
//    std::cout << "Input your command: ";
//    std::getline(std::cin, userInput);
//    std::istringstream iss(userInput);
//    std::string token;
//    while (iss >> std::quoted(token)) {
//        tokens.push_back(token);
//    }
    std::vector<std::pair<class MenuItem, int>> artificialOrder;
    // adds 2 pizzas
    artificialOrder.push_back(std::pair<class MenuItem, int>(parsedVector.at(0), 2));
    // and 1 juice
    artificialOrder.push_back(std::pair<class MenuItem, int>(parsedVector.at(1), 1));
    
    Restaurant riri(parsed, parsedVector, artificialOrder);
    
    riri.printMenu();
    riri.printOrder();
    riri.printStock();
    return 0;
}
