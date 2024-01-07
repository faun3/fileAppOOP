#include "Restaurant.h"

Restaurant::Restaurant() {
    std::set<class MenuItem> emptyMenu;
    std::map<std::string, std::pair<class MenuItem, int>> emptyOrder;
    std::list<class Ingredient> emptyStock;
    
    this->menu = emptyMenu;
    this->order = emptyOrder;
    this->stock = emptyStock;
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
    std::cout << "SERIALIZE IS NOT IMPLEMENTED\n";
}

void Restaurant::deserialize(std::ifstream& file) {
    std::cout << "DESERIALIZE IS NOT IMPLEMENTED";
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
