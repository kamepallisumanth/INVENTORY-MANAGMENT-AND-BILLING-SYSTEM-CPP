#include "Inventory.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

InventorySystem::InventorySystem() {
    loadFromFile();
}

void loadFromFile() {
    ifstream file(filename);
    if (!file.is_open()) return;

    int id, qty;
    string name;
    double price;

    while (file >> id >> quoted(name) >> price >> qty) {
        catalog[id] = {id, name, price, qty};
    }
    file.close();
}

void InventorySystem::saveToFile() {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "[Error] Unable to open database file for saving!\n";
        return;
    }

    for (const auto& pair : catalog) {
        file << pair.second.id << " " 
             << quoted(pair.second.name) << " " 
             << pair.second.price << " " 
             << pair.second.quantity << "\n";
    }
    file.close();
}

void InventorySystem::addOrUpdateProduct(int id, const string &name, double price, int quantity) {
    if (catalog.find(id) != catalog.end()) {
        catalog[id].quantity += quantity;
        catalog[id].price = price;
        cout << "\n[Success] Updated stock and price for ID " << id << " (" << name << ").\n";
    } else {
        catalog[id] = {id, name, price, quantity};
        cout << "\n[Success] Added new product: " << name << " (ID: " << id << ").\n";
    }
    saveToFile();
}

Product* InventorySystem::searchProduct(int id) {
    auto it = catalog.find(id);
    if (it != catalog.end()) {
        return &(it->second);
    }
    return nullptr;
}

void InventorySystem::displayInventory() const {
    if (catalog.empty()) {
        cout << "\n[Notice] Inventory is currently empty.\n";
        return;
    }
    cout << "\n=======================================================\n";
    cout << left << setw(8) << "ID" 
         << setw(22) << "Product Name" 
         << setw(12) << "Price ($)" 
         << setw(10) << "Stock" << "\n";
    cout << "=======================================================\n";
    for (const auto& pair : catalog) {
        cout << left << setw(8) << pair.second.id 
             << setw(22) << pair.second.name 
             << "$" << setw(11) << fixed << setprecision(2) << pair.second.price 
             << setw(10) << pair.second.quantity << "\n";
    }
    cout << "=======================================================\n";
}

void InventorySystem::checkLowStock() const {
    bool lowStockFound = false;
    cout << "\n=======================================================\n";
    cout << "              LOW-STOCK ALERT DASHBOARD                 \n";
    cout << "=======================================================\n";
    for (const auto& pair : catalog) {
        if (pair.second.quantity < 5) {
            if (!lowStockFound) {
                cout << left << setw(8) << "ID" << setw(22) << "Product Name" << setw(10) << "Stock" << "\n";
                cout << "-------------------------------------------------------\n";
                lowStockFound = true;
            }
            cout << left << setw(8) << pair.second.id 
                 << setw(22) << pair.second.name 
                 << setw(10) << pair.second.quantity << " (CRITICAL)\n";
        }
    }
    if (!lowStockFound) {
        cout << "[Status] All inventory items have sufficient stock (>= 5).\n";
    }
    cout << "=======================================================\n";
}

void InventorySystem::processBilling() {
    if (catalog.empty()) {
        cout << "\n[Error] Cannot open billing counter. Inventory is empty.\n";
        return;
    }

    struct BillItem {
        string name;
        int qty;
        double price;
        double total;
    };
    vector<BillItem> cart;

    int choiceId, qty;
    double subtotal = 0.0;
    int totalItemsCount = 0;
    char nextItem;

    cout << "\n--- BILLING COUNTER ---\n";
    do {
        cout << "Enter Product ID: ";
        cin >> choiceId;

        Product* prod = searchProduct(choiceId);
        if (prod == nullptr) {
            cout << "[Error] Product ID not found in inventory!\n";
        } else {
            cout << "Product Found: " << prod->name << " | Stock Available: " << prod->quantity << "\n";
            cout << "Enter Quantity to purchase: ";
            cin >> qty;

            if (qty > prod->quantity) {
                cout << "[Error] Stock low! Maximum available: " << prod->quantity << "\n";
            } else if (qty <= 0) {
                cout << "[Error] Invalid quantity!\n";
            } else {
                prod->quantity -= qty;
                double lineTotal = prod->price * qty;
                subtotal += lineTotal;
                totalItemsCount += qty;
                cart.push_back({prod->name, qty, prod->price, lineTotal});
                cout << "-> Added " << qty << " x " << prod->name << " ($" << lineTotal << ") to cart.\n";
            }
        }

        cout << "Add another product to cart? (y/n): ";
        cin >> nextItem;
    } while (nextItem == 'y' || nextItem == 'Y');

    if (!cart.empty()) {
        saveToFile();

        char isMerchantChoice;
        bool isRegisteredMerchant = false;
        double merchantDiscountRate = 0.0;

        cout << "\nIs the buyer a registered merchant/wholesaler? (y/n): ";
        cin >> isMerchantChoice;

        if (isMerchantChoice == 'y' || isMerchantChoice == 'Y') {
            isRegisteredMerchant = true;
            if (totalItemsCount >= 10 || subtotal >= 200.0) {
                merchantDiscountRate = 0.10;
                cout << "[Merchant Benefit] Bulk threshold met! Applied 10% Wholesale Discount.\n";
            } else {
                merchantDiscountRate = 0.05;
                cout << "[Merchant Benefit] Applied 5% Registered Merchant Discount.\n";
            }
        }

        double discountAmount = subtotal * merchantDiscountRate;
        double discountedSubtotal = subtotal - discountAmount;
        double taxRate = isRegisteredMerchant ? 0.02 : 0.05;
        double tax = discountedSubtotal * taxRate;
        double grandTotal = discountedSubtotal + tax;

        time_t now = time(nullptr);
        char timestamp[100];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", localtime(&now));

        string receiptName = "Invoice_" + string(timestamp) + ".txt";
        ofstream receipt(receiptName);

        cout << "\n=======================================================\n";
        cout << "                 OFFICIAL RECEIPT                      \n";
        cout << " Timestamp: " << timestamp << "\n";
        cout << " Buyer Type: " << (isRegisteredMerchant ? "Registered Merchant" : "Regular Customer") << "\n";
        cout << "=======================================================\n";
        cout << left << setw(20) << "Item" 
             << setw(8) << "Qty" 
             << setw(12) << "Unit Price" 
             << setw(10) << "Total" << "\n";
        cout << "-------------------------------------------------------\n";

        if (receipt.is_open()) {
            receipt << "=======================================================\n";
            receipt << "                 OFFICIAL RECEIPT                      \n";
            receipt << " Timestamp: " << timestamp << "\n";
            receipt << " Buyer Type: " << (isRegisteredMerchant ? "Registered Merchant" : "Regular Customer") << "\n";
            receipt << "=======================================================\n";
            receipt << left << setw(20) << "Item" 
                    << setw(8) << "Qty" 
                    << setw(12) << "Unit Price" 
                    << setw(10) << "Total" << "\n";
            receipt << "-------------------------------------------------------\n";
        }

        for (const auto& item : cart) {
            cout << left << setw(20) << item.name 
                 << setw(8) << item.qty 
                 << "$" << setw(11) << fixed << setprecision(2) << item.price 
                 << "$" << item.total << "\n";

            if (receipt.is_open()) {
                receipt << left << setw(20) << item.name 
                        << setw(8) << item.qty 
                        << "$" << setw(11) << fixed << setprecision(2) << item.price 
                        << "$" << item.total << "\n";
            }
        }

        cout << "-------------------------------------------------------\n";
        cout << "Subtotal:            $" << fixed << setprecision(2) << subtotal << "\n";
        if (merchantDiscountRate > 0) {
            cout << "Merchant Discount:  -$" << discountAmount << "\n";
            cout << "Discounted Subtotal: $" << discountedSubtotal << "\n";
        }
        cout << "GST (" << (taxRate * 100) << "%):       $" << tax << "\n";
        cout << "Grand Total:         $" << grandTotal << "\n";
        cout << "=======================================================\n";
        cout << "[Success] Saved invoice file: " << receiptName << "\n";

        if (receipt.is_open()) {
            receipt << "-------------------------------------------------------\n";
            receipt << "Subtotal:            $" << fixed << setprecision(2) << subtotal << "\n";
            if (merchantDiscountRate > 0) {
                receipt << "Merchant Discount:  -$" << discountAmount << "\n";
                receipt << "Discounted Subtotal: $" << discountedSubtotal << "\n";
            }
            receipt << "GST (" << (taxRate * 100) << "%):       $" << tax << "\n";
            receipt << "Grand Total:         $" << grandTotal << "\n";
            receipt << "======================================================-\n";
            receipt.close();
        }
    }
}

void InventorySystem::reprintReceipt() const {
    string invoiceId;
    cout << "\n--- REPRINT RECEIPT ---\n";
    cout << "Enter the Invoice Timestamp / ID (e.g., 2026-10-18_09-43-15): ";
    cin >> invoiceId;

    string filename = "Invoice_" + invoiceId + ".txt";
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "[Error] Invoice file '" << filename << "' could not be found!\n";
        return;
    }

    cout << "\n--- RETRIEVING INVOICE: " << filename << " ---\n";
    string line;
    while (getline(file, line)) {
        cout << line << "\n";
    }
    file.close();
}
