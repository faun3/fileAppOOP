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
        out << "\n---Ingredient---\n";
        out << "Name: " << i.name << "\n";
        out << "Quantitiy: " << i.quantity << "\n";
        out << "\n";
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

class ITextReadable {
public:
    virtual std::vector<Ingredient> readText(const std::string& filename) = 0;
    virtual ~ITextReadable() {}
};

class IngredientParser : ITextReadable {
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


int main(int argc, const char * argv[]) {
    IngredientParser ip;
    Ingredient i1("Tomato", 600);
    // std::cout << i1;
    std::vector<Ingredient> parsed;
    parsed = ip.readText("ingredients.txt");
    
    for (const auto& ing : parsed) {
        std::cout << ing;
    }
    
    return 0;
}
