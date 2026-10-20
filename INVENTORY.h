#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <unordered_map>
#include <vector>

struct Product {
    int id;
    std::string name;
    double price;
    int quantity;
};

class InventorySystem {
private:
    std::unordered_map<int, Product> catalog;
    const std::string filename = "inventory_db.txt";

    void loadFromFile();
    void saveToFile();

public:
    InventorySystem();
    void addOrUpdateProduct(int id, const std::string &name, double price, int quantity);
    Product* searchProduct(int id);
    void displayInventory() const;
    void checkLowStock() const;
    void processBilling();
    void reprintReceipt() const;
    
    // NEW FEATURE: Search products by name (substring matching)
    void searchByName(const std::string &keyword) const;
};

bool authenticateUser(std::string &loggedInUser, std::string &role);

#endif // INVENTORY_H

bool authenticateUser(std::string &loggedInUser, std::string &role);

#endif // INVENTORY_H
