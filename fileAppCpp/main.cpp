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

class IngredientParser {
private:
    static std::vector<std::string> splitter(const std::string& line, const char delimiter) {
        std::vector<std::string> split;
        
        std::istringstream iss(line);
        std::string token;

        while (std::getline(iss, token, delimiter)) {
            split.push_back(token);
        }
        
        return split;
    }
public:
    static std::vector<class Ingredient> readText(const std::string& filename) {
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
        
        return parsedIngredientList;
    }
    
    static std::vector<class Ingredient> parseCsv(const std::string& filename) {
        std::vector<class Ingredient> parsedIngredientList;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "File mishap" << std::endl;
            return parsedIngredientList;
        }
        
        std::string line;
        
        while (std::getline(file, line)) {
            
            std::vector<std::string> exploded = IngredientParser::splitter(line, delimiter);
            // no error handling here
            std::string unquoted = exploded.at(0);
            if (!unquoted.empty() && unquoted.front() == '\"') {
                unquoted.erase(unquoted.begin());
            }
            
            if (!unquoted.empty() && unquoted.back() == '\"') {
                unquoted.pop_back();
            }
            
            int qty;
            try {
                qty = stoi(exploded.at(1));
            }
            catch (std::invalid_argument e) {
                qty = 0;
            }
            class Ingredient temp(unquoted, qty);
            parsedIngredientList.push_back(temp);
        }
    
        return parsedIngredientList;
    }
};

class MenuItemParser {
public:
    static std::vector<class MenuItem> parseTextFile(const std::string& filename) {
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
    if (argc == 1) {
        std::cout << "No data files were specified. Using defaults.\n";
        // std::ifstream ingredientsFile("ing.bin", std::ios::binary);
        // std::ifstream menuItemFile("menuItem.bin", std::ios::binary);
        
        std::vector<class Ingredient> stock;
    }
    else {
        if (argc == 3) {
            
        }
        
        else {
            std::cout << "Invalid number of arguments specified.\n Please use 2 .txt or .bin files. The first file should contain information about the restaurant's stock, and the second file should contain information about the restaurant's menu.\nThe file containing ingredient data can also be a .csv.\nUsing default data.";
        }
    }
    
    // std::vector<class Ingredient> parsed;
    // parsed = IngredientParser::readText(argv[1]);
    
    // std::vector<class MenuItem> parsedVector;
    // parsedVector = MenuItemParser::parseTextFile(argv[2]);
    
    std::vector<std::pair<class MenuItem, int>> artificialOrder;
    // adds 2 pizzas
    // artificialOrder.push_back(std::pair<class MenuItem, int>(parsedVector.at(0), 2));
    // and 1 juice
    // artificialOrder.push_back(std::pair<class MenuItem, int>(parsedVector.at(1), 1));
    
    Restaurant riri;
    
    enum mode {
        read,
        write
    };
    
    mode mode = mode::read;
    if (mode == mode::read) {
        std::ifstream file("restaurant.bin", std::ios::binary);
        riri.deserialize(file);
        file.close();
    }
    else {
        std::ofstream file("restaurant.bin", std::ios::binary);
        riri.serialize(file);
        file.close();
    }
    
    riri.printMenu();
    riri.printOrder();
    riri.printStock();
    return 0;
}
