#pragma once
#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>
#include <utility>
#include <fstream>
#include <iterator>
#include "globals.h"
#include "ISerializable.h"
#include "MenuItem.h"
#include "Ingredient.h"

class Restaurant : ISerializable {
private:
    std::set<class MenuItem> menu;
    std::map<std::string, std::pair<class MenuItem, int>> order;
    std::list<class Ingredient> stock;
public:
    Restaurant();
    Restaurant(std::vector<class Ingredient>, std::vector<class MenuItem>, std::vector<std::pair<class MenuItem, int>> order);
    void serialize(std::ofstream& file) const;
    void deserialize(std::ifstream& file);
    void printMenu() const;
    void printOrder() const;
    void printStock() const;
    std::list<class Ingredient> getStock() const;
    void printStockDelta(std::list<class Ingredient> stockCopy, std::ofstream& logFile) const;
    void reduceStock();
    void addToOrder(class MenuItem item, int quantity);
    class MenuItem findInMenu(std::string menuItemName);
    void clearOrder();
    std::list<class Ingredient> copyStock() const;
    double getOrderPrice() const;
};
