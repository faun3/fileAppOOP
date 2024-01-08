#include "Restaurant.h"
#include "ISerializable.h"

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
    for (const auto& oi : this->order) {
        std::cout << oi.second.first.getName() << " x " << oi.second.second << "\n";
    }
}

void Restaurant::printStock() const {
    std::cout << "\n---Stock---\n";
    for (const auto& i : this->stock) {
        std::cout << i.getName() << " " << i.getQuantity() << "\n";
    }
}
