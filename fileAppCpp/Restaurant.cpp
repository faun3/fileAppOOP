#include "Restaurant.h"
#include "ISerializable.h"
#include <set>
#include <algorithm>

Restaurant::Restaurant() {
    std::set<class MenuItem> emptyMenu;
    std::map<std::string, std::pair<class MenuItem, int>> emptyOrder;
    std::list<class Ingredient> emptyStock;
    
    this->stock = emptyStock;
    this->menu = emptyMenu;
    this->order = emptyOrder;
}

Restaurant::Restaurant(std::vector<class Ingredient> stock, std::vector<class MenuItem> menu, std::vector<std::pair<class MenuItem, int>> order) {
    std::set<class MenuItem> newMenu(menu.begin(), menu.end());
    this->menu = newMenu;
    std::copy(stock.begin(), stock.end(), std::back_inserter(this->stock));
    
    for (const auto& pair : order) {
        this->order[pair.first.getName()] = pair;
    }
}

void Restaurant::serialize(std::ofstream& file) const {
    if (!file.is_open()) return;
    
    size_t stockLength = this->stock.size();
    file.write(reinterpret_cast<char*>(&stockLength), sizeof(stockLength));
    for (const auto& i : this->stock) {
        i.serialize(file);
    }
    
    size_t menuItemLength = this->menu.size();
    file.write(reinterpret_cast<char*>(&menuItemLength), sizeof(menuItemLength));
    for (const auto& mi : this->menu) {
        mi.serialize(file);
    }
    
    size_t orderLength = this->order.size();
    file.write(reinterpret_cast<char*>(&orderLength), sizeof(orderLength));
    for (const auto& oi : this->order) {
        oi.second.first.serialize(file);
        int number = oi.second.second;
        file.write(reinterpret_cast<char*>(&number), sizeof(number));
    }
}

void Restaurant::deserialize(std::ifstream& file) {
    if (!file.is_open()) return;
    
    size_t stockLength;
    size_t menuItemsLength;
    size_t orderLength;
    std::vector<class Ingredient> parsedIngredients;
    std::vector<class MenuItem> parsedMenuItems;
    std::vector<std::pair<class MenuItem, int>> parsedOrderItems;
    
    this->stock.clear();
    file.read(reinterpret_cast<char*>(&stockLength), sizeof(stockLength));
    for (size_t i = 0; i < stockLength; i++) {
        class Ingredient temp;
        temp.deserialize(file);
        parsedIngredients.push_back(temp);
    }
    
    file.read(reinterpret_cast<char*>(&menuItemsLength), sizeof(menuItemsLength));
    for (size_t i = 0; i < menuItemsLength; i++) {
        class MenuItem temp;
        temp.deserialize(file);
        parsedMenuItems.push_back(temp);
    }
    
    file.read(reinterpret_cast<char*>(&orderLength), sizeof(orderLength));
    for (size_t i = 0; i < orderLength; i++) {
        class MenuItem temp;
        int quantity;
        temp.deserialize(file);
        file.read(reinterpret_cast<char*>(&quantity), sizeof(quantity));
        std::pair<class MenuItem, int> pair(temp, quantity);
        parsedOrderItems.push_back(pair);
    }
    
    std::copy(parsedIngredients.begin(), parsedIngredients.end(), std::back_inserter(this->stock));
    std::set<class MenuItem> newMenu(parsedMenuItems.begin(), parsedMenuItems.end());
    this->menu = newMenu;
    std::map<std::string, std::pair<class MenuItem, int>> newOrder;
    for (const auto& pair : parsedOrderItems) {
        this->order[pair.first.getName()] = pair;
    }
}

void Restaurant::printMenu() const {
    std::cout << "\n---Menu---\n";
    for (const auto& mi : this->menu) {
        std::cout << mi.getName() << "\n";
    }
}

void Restaurant::printOrder() const {
    std::cout << "\n---Order---\n";
    double total = 0;
    if (order.size() > 0) {
        for (const auto& oi : this->order) {
            double price = oi.second.first.getPrice() * oi.second.second;
            total += price;
            std::cout << oi.second.first.getName() << " x " << oi.second.second;
            std::cout << " -- " << price << "\n";
        }
        std::cout << "Total: " << total << "\n";
    }
    else std::cout << "Nothing yet.\n";
}

void Restaurant::printStock() const {
    std::cout << "\n---Stock---\n";
    for (const auto& i : this->stock) {
        std::cout << i.getName() << " " << i.getQuantity() << "\n";
    }
}

void Restaurant::clearOrder() {
    this->order.clear();
}

class MenuItem Restaurant::findInMenu(std::string menuItemName) {
    class MenuItem found;
    for (const auto& i : this->menu) {
        if (i.getName() == menuItemName) {
            found = i;
            return found;
        }
    }
    return found;
}

void Restaurant::addToOrder(class MenuItem item, int quantity) {
    if (this->order.find(item.getName()) == this->order.end()) {
        this->order[item.getName()] = std::pair<class MenuItem, int>(item, quantity);
    }
    else {
        this->order[item.getName()].second += quantity;
    }
}

std::list<class Ingredient> Restaurant::copyStock() const {
    std::list<class Ingredient> copy = this->stock;
    return copy;
}

void Restaurant::reduceStock() {
    std::list<class Ingredient> stockCopy = this->stock;
    for (const auto& keyValue : this->order) {
        class MenuItem mi = keyValue.second.first;
        int qty = keyValue.second.second;
        for (auto& ing : mi.getIngredients()) {
            int menuItemQty = ing.getQuantity();
            bool found = false;
            for (auto& stockIng : stockCopy) {
                int inStockQty = stockIng.getQuantity();
                if (stockIng.getName() == ing.getName()) {
                    found = true;
                    if (inStockQty >= (menuItemQty * qty)) {
                        int newQty = inStockQty - (menuItemQty * qty);
                        stockIng.setQuantity(newQty);
                    }
                    else {
                        throw std::invalid_argument("Not enough stock to do that!");
                    }
                }
            }
            if (!found) throw std::invalid_argument("Some ingredients are not in stock!");
        }
    }
    this->stock = stockCopy;
}

std::list<class Ingredient> Restaurant::getStock() const {
    return this->stock;
}

void Restaurant::printStockDelta(std::list<class Ingredient> stockCopy, std::ofstream& logFile) const {
    for (const auto& si : this->stock) {
        for (const auto& sCpyItem : stockCopy) {
            if (sCpyItem.getName() == si.getName() && sCpyItem.getQuantity() != si.getQuantity()) {
                logFile << "delta " << si.getName() << " " << si.getQuantity() - sCpyItem.getQuantity() << "\n";
            }
        }
    }
}

double Restaurant::getOrderPrice() const {
    double total = 0;
    for (const auto& oi : this->order) {
        double price = oi.second.first.getPrice() * oi.second.second;
        total += price;
    }
    return total;
}
