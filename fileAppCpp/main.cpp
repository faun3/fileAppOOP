#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

class Ingredient {
private:
    std::string name = "Unknown";
    int quantity = 0;
public:
    Ingredient(std::string name, int quantity) {
        this->name = name;
        this->quantity = quantity;
    }
    friend std::ostream& operator<<(std::ostream& out, const Ingredient& i) {
        out << "---Ingredient---\n";
        out << "Name: " << i.name << "\n";
        out << "Quantitiy: " << i.quantity << "\n";
        return out;
    }
};

class MenuItem {
private:
    std::string name = "Unknown";
    double price = 0;
    std::vector<Ingredient> ingredients;
public:
    MenuItem(std::string name, double price, std::vector<Ingredient> ingredients) {
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
    virtual std::vector<Ingredient> readText(const std::string& filename) = 0;
    virtual ~ITextReadableIngredient() {}
};

class IngredientParser : ITextReadableIngredient {
public:
    std::vector<Ingredient> readText(const std::string& filename) override {
        std::vector<Ingredient> parsedIngredientList;
        std::ifstream file(filename);
        
        if (!file.is_open()) {
            std::cout << "File mishap" << std::endl;
            return parsedIngredientList;
        }
        std::string parsedName;
        int parsedQuantity;
        
        while (file >> parsedName >> parsedQuantity) {
            Ingredient toPush(parsedName, parsedQuantity);
            parsedIngredientList.push_back(toPush);
        }
        
        file.close();
        return parsedIngredientList;
    }
};

class ITextReadableMenuItem {
public:
    virtual std::vector<MenuItem> parseTextFile(const std::string& filename) = 0;
    virtual ~ITextReadableMenuItem() {}
};

class MenuItemParser : ITextReadableMenuItem {
public:
    std::vector<MenuItem> parseTextFile(const std::string& filename) override {
        std::vector<MenuItem> result;
        
        std::ifstream file(filename);
        if (!file.is_open()) return result;
        
        std::string menuItemName;
        double menuItemPrice;
        int ingredientListLength;
        std::vector<Ingredient> ingredients;
        std::string ingredientName;
        int ingredientQuantity;
        
        while (file >> menuItemName >> menuItemPrice >> ingredientListLength) {
            for (int step = 0; step < ingredientListLength; step++) {
                file >> ingredientName >> ingredientQuantity;
                Ingredient toPush(ingredientName, ingredientQuantity);
                ingredients.push_back(toPush);
            }
            MenuItem parsedMenuItem(menuItemName, menuItemPrice, ingredients);
            result.push_back(parsedMenuItem);
            ingredients.clear();
        }
        
        return result;
    }
};

int main(int argc, const char * argv[]) {
    IngredientParser ip;
    Ingredient i1("Tomato", 600);
    // std::cout << i1;
    std::vector<Ingredient> parsed;
    parsed = ip.readText("ingredients.txt");
    
    for (const auto& ing : parsed) {
        std::cout << ing << "\n";
    }
    
    MenuItem mi1("Pizza", 65.98, parsed);
    //std::cout << mi1 << std::endl;
    
    MenuItemParser mip;
    std::vector<MenuItem> parsedVector;
    parsedVector = mip.parseTextFile("menuItems.txt");
    
    for (const auto& mi : parsedVector) {
        std::cout << mi << "\n";
    }
    
    return 0;
}
