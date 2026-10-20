#include <iostream>
#include "INVENTORY.h"

using namespace std;

int main() {
    InventorySystem store;
    string loggedInUser = "";
    string role = "";

    if (!authenticateUser(loggedInUser, role)) {
        return 0;
    }

    if (role == "ADMIN") {
        store.checkLowStock();
    }

    int choice;
    do {
        cout << "\n===============================\n";
        cout << "  MAIN MENU (" << loggedInUser << " - " << role << ")\n";
        cout << "===============================\n";
        
        if (role == "ADMIN") {
            cout << "1. Add / Restock Product\n";
            cout << "2. View Inventory Catalog\n";
            cout << "3. Search Product by Name\n";
            cout << "4. Open Billing Counter\n";
            cout << "5. View Low-Stock Dashboard\n";
            cout << "6. Reprint Receipt by ID\n";
            cout << "7. View System Audit Trail Log\n";
            cout << "8. Logout & Exit\n";
        } else {
            cout << "1. View Inventory Catalog\n";
            cout << "2. Search Product by Name\n";
            cout << "3. Open Billing Counter\n";
            cout << "4. Reprint Receipt by ID\n";
            cout << "5. Logout & Exit\n";
        }
        cout << "Enter your choice: ";
        
        while (!(cin >> choice)) {
            cout << "[Error] Please enter a valid menu number option: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        if (role == "ADMIN") {
            if (choice == 1) {
                int id;
                cout << "\nEnter Product ID (Integer): "; 
                while (!(cin >> id)) {
                    cout << "[Error] Invalid ID. Enter a valid integer: ";
                    cin.clear();
                    cin.ignore(10000, '\n');
                }

                Product* existingProd = store.searchProduct(id);

                if (existingProd != nullptr) {
                    int restockQty;
                    double price = existingProd->price;
                    char changePrice;

                    cout << "\n[Product Found in Inventory]\n";
                    cout << "Name: " << existingProd->name << "\n";
                    cout << "Current Stock: " << existingProd->quantity << "\n";
                    cout << "Current Price: $" << existingProd->price << "\n";
                    
                    cout << "Do you want to update the price? (y/n): ";
                    cin >> changePrice;
                    if (changePrice == 'y' || changePrice == 'Y') {
                        cout << "Enter new price ($): ";
                        while (!(cin >> price) || price < 0) {
                            cout << "[Error] Invalid price. Enter a valid amount: ";
                            cin.clear();
                            cin.ignore(10000, '\n');
                        }
                    }

                    cout << "Enter quantity to add (restock): ";
                    while (!(cin >> restockQty) || restockQty <= 0) {
                        cout << "[Error] Invalid quantity. Enter a positive number: ";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }

                    store.addOrUpdateProduct(id, existingProd->name, price, restockQty, loggedInUser);
                } else {
                    string name;
                    double price;
                    int qty;
                    cout << "\n[New Product Registration]\n";
                    cout << "Enter Product Name: "; cin >> name;
                    cout << "Enter Price ($): "; 
                    while (!(cin >> price) || price < 0) {
                        cout << "[Error] Invalid price. Enter a valid amount: ";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    cout << "Enter Quantity to add: "; 
                    while (!(cin >> qty) || qty <= 0) {
                        cout << "[Error] Invalid quantity. Enter a positive number: ";
                        cin.clear();
                        cin.ignore(10000, '\n');
                    }
                    store.addOrUpdateProduct(id, name, price, qty, loggedInUser);
                }
            } else if (choice == 2) {
                store.displayInventory();
            } else if (choice == 3) {
                string keyword;
                cout << "\nEnter product name keyword to search: ";
                cin >> keyword;
                store.searchByName(keyword);
            } else if (choice == 4) {
                store.processBilling(loggedInUser);
            } else if (choice == 5) {
                store.checkLowStock();
            } else if (choice == 6) {
                store.reprintReceipt();
            } else if (choice == 7) {
                store.viewAuditLog();
            } else if (choice == 8) {
                cout << "\nLogging out... Goodbye, " << loggedInUser << "!\n";
                break;
            } else {
                cout << "\n[Error] Invalid option selected!\n";
            }
        } else { // CASHIER MODE
            if (choice == 1) {
                store.displayInventory();
            } else if (choice == 2) {
                string keyword;
                cout << "\nEnter product name keyword to search: ";
                cin >> keyword;
                store.searchByName(keyword);
            } else if (choice == 3) {
                store.processBilling(loggedInUser);
            } else if (choice == 4) {
                store.reprintReceipt();
            } else if (choice == 5) {
                cout << "\nLogging out... Goodbye, " << loggedInUser << "!\n";
                break;
            } else {
                cout << "\n[Error] Invalid option selected!\n";
            }
        }
    } while (true);

    return 0;
}
