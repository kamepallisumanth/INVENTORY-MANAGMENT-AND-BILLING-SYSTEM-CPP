#ifndef INVENTORY_H
#define INVENTORY_H

#include <string>
#include <unordered_map>
#include <vector>

struct Product {
    int id;
   std:: string name;
    double price;
    int quantity;
}

class InventorySystem {
private:
    
    std::unorderd_map<int, Product> catalog;
    const std::string filename = "inventory_db.txt";

    void loadFromFile();
    void saveToFile();

public:
    InventorySystem()
    void addOrUpdateProduct(int id, const  std::string &name, double price, int quantity);
    Product* searchProduct(int id);
    void displayInventory() const;
    void checkLowStock() const;
    void processBilling();
    void reprintReceipt() const;
} 

bool authenticateUser(string &loggedInUser, std:: string &role);

#endif // INVENTORY_H
