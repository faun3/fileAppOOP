#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <sstream>
#include <queue>

#include "globals.h"
#include "CheckExtension.h"
#include "ISerializable.h"
#include "Ingredient.h"
#include "MenuItem.h"
#include "Restaurant.h"
#include "Parser.h"
#include "Logger.h"

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
        file.close();
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
        file.close();
        return result;
    }
};


int main(int argc, const char * argv[]) {
    Restaurant riri;
    std::queue<std::string> queue;
    
    if (argc == 1) {
        std::cout << "No data files were specified. Using defaults.\n";
        std::ifstream file("restaurant.bin", std::ios::binary);
        riri.deserialize(file);
        file.close();
    }
    else if (argc == 2) {
        std::string fileExt = CheckExtension::getExtension(argv[2]);
        if (fileExt != "bin") {
            std::cout << "File is not binary. Using defaults.\n";
            std::ifstream file("restaurant.bin", std::ios::binary);
            riri.deserialize(file);
            file.close();
        }
    }
    else {
        if (argc == 3) {
            try {
                std::string firstFileExt = CheckExtension::getExtension(argv[1]);
                std::string secondFileExt = CheckExtension::getExtension(argv[2]);
                std::vector<std::pair<class MenuItem, int>> emptyOrder;
                if (firstFileExt == "bin" && secondFileExt == "bin") {
                    std::cout << "Reading data from two binary files...";
                    std::ifstream ingredientsFile(argv[1], std::ios::binary);
                    std::ifstream menuItemFile(argv[2], std::ios::binary);
                    
                    size_t ingLength;
                    std::vector<class Ingredient> ings;
                    ingredientsFile.read(reinterpret_cast<char*>(&ingLength), sizeof(ingLength));
                    for (size_t i = 0; i < ingLength; i++) {
                        class Ingredient temp;
                        temp.deserialize(ingredientsFile);
                        ings.push_back(temp);
                    }
                    
                    size_t misLength;
                    std::vector<class MenuItem> mis;
                    menuItemFile.read(reinterpret_cast<char*>(&misLength), sizeof(misLength));
                    for (size_t i = 0; i < misLength; i++) {
                        class MenuItem temp;
                        temp.deserialize(menuItemFile);
                        mis.push_back(temp);
                    }
                    
                    Restaurant copy(ings, mis, emptyOrder);
                    riri = copy;
                    std::cout << "Data loaded.\n";
                    
                    ingredientsFile.close();
                    menuItemFile.close();
                }
                else if (firstFileExt == "csv") {
                    std::cout << "Reading data from a csv and a txt file...\n";
                    std::vector<class Ingredient> ings = IngredientParser::parseCsv(argv[1]);
                    std::vector<class MenuItem> mis = MenuItemParser::parseTextFile(argv[2]);
                    
                    Restaurant copy(ings, mis, emptyOrder);
                    riri = copy;
                    std::cout << "Data loaded.\n";
                }
                else if (firstFileExt == "txt") {
                    std::cout << "Reading data from two txt files...\n";
                    std::vector<class Ingredient> ings = IngredientParser::readText(argv[1]);
                    std::vector<class MenuItem> mis = MenuItemParser::parseTextFile(argv[2]);
                    
                    Restaurant copy(ings, mis, emptyOrder);
                    riri = copy;
                    std::cout << "Data loaded.\n";
                }
            }
            catch (std::invalid_argument e) {
                std::cout << e.what() << "\n";
                std::ifstream file("restaurant.bin", std::ios::binary);

                riri.deserialize(file);
                
                file.close();
            }
        }
        else {
            std::cout << "Invalid number of arguments specified.\n Please use 2 .txt or .bin files. The first file should contain information about the restaurant's stock, and the second file should contain information about the restaurant's menu.\nThe file containing ingredient data can also be a .csv.\nUsing default data.";
        }
    }

    riri.printMenu();
    riri.printOrder();
    riri.printStock();
    
    std::list<class Ingredient> copyForDeltas = riri.copyStock();
    double dailySales = 0;
    
    std::ofstream logFile("logs.txt", std::ios::app);
    std::ofstream salesLog("sales.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cout << "Log file could not open. Running without logs...\n";
    }
    if (!salesLog.is_open()) {
        std::cout << "Log file for sales could not open. Running without sales logs...\n";
    }
    
    bool running = true;
    while (running) {
        std::vector<std::string> tokenizedLine;
        std::string line;
        std::getline(std::cin, line);
        if (line != "") {
            logFile << Logger::appendTime(line);
        }
        tokenizedLine = Parser::splitCommand(line);
        if (tokenizedLine.empty()) {
            std::cout << "Invalid command!\nUse \"help\" to print the help message.\n";
        }
        else if (tokenizedLine[0] == "leave") {
            logFile.close();
            
            riri.printStockDelta(copyForDeltas, "stockDelta.txt");
            
            std::string sales = "Total volume: " + std::to_string(dailySales);
            salesLog << Logger::appendTime(sales);
            salesLog.close();
            
            std::cout << "\nbye\n";
            running = false;
        }
        else if (tokenizedLine[0] == "order") {
            if (tokenizedLine.size() == 1) {
                riri.printOrder();
            }
            else if (tokenizedLine[1] == "add") {
                if (tokenizedLine.size() != 4) {
                    std::cout << "Incorrect usage.\n";
                }
                else {
                    std::string menuItemName = tokenizedLine[2];
                    int quantity;
                    try {
                        quantity = stoi(tokenizedLine[3]);
                        if (quantity <= 0) {
                            std::cout << "Quantity cannot be negative or 0.\n";
                        }
                        else {
                            class MenuItem it = riri.findInMenu(menuItemName);
                            if (it.getName() == "Unknown") {
                                std::cout << "That item is not in the menu!\n";
                            }
                            else {
                                std::cout << "Order updated.\n";
                                riri.addToOrder(it, quantity);
                            }
                        }
                    }
                    catch (std::invalid_argument e) {
                        std::cout << "";
                    }
                }
            }
            else if (tokenizedLine[1] == "place") {
                try {
                    riri.reduceStock();
                    dailySales += riri.getOrderPrice();
                    riri.clearOrder();
                    std::cout << "Order placed successfully.\n";
                }
                catch (std::invalid_argument e) {
                    std::cout << e.what() << "\n";
                }
            }
            else if (tokenizedLine[1] == "clear") {
                riri.clearOrder();
                std::cout << "Cleared order.\n";
            }
            
        }
        else if (tokenizedLine[0] == "stock") {
            riri.printStock();
        }
        else if (tokenizedLine[0] == "menu") {
            riri.printMenu();
        }
        else if (tokenizedLine[0] == "save") {
            std::ofstream file("restaurant.bin", std::ios::binary);
            if (!file.is_open()) {
                std::cout << "File didn't open correctly.\n";
            }
            else {
                std::cout << "Saving...\n";
                riri.serialize(file);
                file.close();
                std::cout << "Data saved.\n";
            }
        }
        else if (tokenizedLine[0] == "help") {
            std::cout << "\n---COMMANDS---\n";
            std::cout << "leave - kills the program\n";
            std::cout << "save - saves app data to the default save location\n";
            std::cout << "order - prints the order\n";
            std::cout << "\tshow - prints the current order to the console\n";
            std::cout << "\tadd <menu item name> <quantity> - adds the specified menu item to the order\n";
            std::cout << "\tplace - places the order\n";
            std::cout << "\tclear - empties the current order\n";
            std::cout << "stock - prints the ingredients in the stock\n";
            std::cout << "menu - prints the menu\n";
            std::cout << "help - prints this message\n";
        }
        else {
            std::cout << "Invalid command.\nUse \"help\" to print the help message.\n";
        }
    }
    return 0;
}
