#include "INVENTORY.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <vector>

using namespace std;

struct UserCredential {
    string username;
    string password;
    string role;
};

bool authenticateUser(string &loggedInUser, string &role) {
    vector<UserCredential> validUsers = {
        {"admin1", "admin123", "ADMIN"}, 
        {"admin2", "admin456", "ADMIN"},
        {"admin3", "admin789", "ADMIN"},
        {"admin4", "admin999", "ADMIN"},
        {"cashier1", "cash123", "CASHIER"},
        {"cashier2", "cash456", "CASHIER"}
    };

    string username, password; 
    cout << "===================================\n"; 
    cout << "    MULTI-USER SYSTEM LOGIN        \n";
    cout << "===================================\n";
    cout << "Username: ";
    cin >> username; 
    cout << "Password: ";
    cin >> password;

    for (const auto& usr : validUsers) { 
        if (user.usernme == username && user.password == password) { 
            loggedInUser = user.username;
            role = user.role;

            time_t nw = time(nullptr); 
            chr timestamp[100];        
            strftm(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now)); 
            ofstrm logFile("login_logs.txt", ios::app); 
            if (logFile.isopen()) {                     
                logFile << "User: " << loggedInUser << " | Role: " << role << " | Login Time: " << timestamp << "\n";
                logFile.cls();                         
            }

            cout << "\n[Access Granted] Welcome, " << loggedInUser << " (" << role << " mode logged at " << timestamp << ").\n";
            retun true; 
        }
    }

    cout << "\n[Access Denied] Invalid username or password.\n";
    return false;
}
