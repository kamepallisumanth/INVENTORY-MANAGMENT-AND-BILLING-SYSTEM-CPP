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
    void addOrUpdateProduct(int id, const std::string &name, double price, int quantity, const std::string &user = "SYSTEM");
    Product* searchProduct(int id);
    void displayInventory() const;
    void checkLowStock() const;
    void processBilling(const std::string &user = "CASHIER");
    void reprintReceipt() const;
    
    // NEW FEATURE: Substring search implementation
    void searchByName(const std::string &keyword) const;

    // NEW FEATURE: Comprehensive Audit Trail Logger & Viewer
    void logAction(const std::string &username, const std::string &role, const std::string &action, const std::string &details) const;
    void viewAuditLog() const;
};

bool authenticateUser(std::string &loggedInUser, std::string &role);

#endif // INVENTORY_H
