#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

#include "globals.h"
#include "CheckExtension.h"
#include "ISerializable.h"
#include "Ingredient.h"

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

//class Ingredient {
//private:
//    std::string name = "Unknown";
//    int quantity = 0;
//public:
//    Ingredient(std::string name, int quantity) {
//        this->name = name;
//        this->quantity = quantity;
//    }
//    friend std::ostream& operator<<(std::ostream& out, const Ingredient& i) {
//        out << "---Ingredient---\n";
//        out << "Name: " << i.name << "\n";
//        out << "Quantitiy: " << i.quantity << "\n";
//        return out;
//    }
//};

class MenuItem {
private:
    std::string name = "Unknown";
    double price = 0;
    std::vector<class Ingredient> ingredients;
public:
    MenuItem(std::string name, double price, std::vector<class Ingredient> ingredients) {
        this->name = name;
        this->price = price;
        this->ingredients = ingredients;
    }
    friend std::ostream& operator<<(std::ostream& out, const MenuItem& m) {
        out << "---Menu Item---\n";
        out << "Name: " << m.name << "\n";
        out << "Price: " << m.price << "\n";
        out << "Ingredients: \n";
        for (const auto& ing : m.ingredients) {
            out << ing;
        }
        return out;
    }
};

class IBinaryWritable {
public:
    virtual bool writeBinary(const std::string& filename) = 0;
    virtual ~IBinaryWritable() {}
};

class IBinaryReadable {
public:
    virtual bool readBinary(const std::string& filename) = 0;
    virtual ~IBinaryReadable() {}
};

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
    
    for (const auto& mi : parsedVector) {
        std::cout << mi << "\n";
    }
    
    std::cout << "\n\n\n";
    
    for (const auto& exts : allowedExtensions) {
        std::cout << exts << " ";
    }
    std::cout << "\n";
    
    std::cout << "First file: " << CheckExtension::checkExtension(argv[1]) << "\n";
    std::cout << "Second file: " << CheckExtension::checkExtension(argv[2]) << "\n";

    
    
    return 0;
}
