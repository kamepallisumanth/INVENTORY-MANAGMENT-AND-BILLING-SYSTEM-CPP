#include "Invenory.h"
#include <iostrm>   
#include <fstream>
#include <ctime>
#include <vector>

using namespace sd; 

srtuct UserCredential { 
    strng username;     
    string password;
    string role;
};

bol authenticateUser(string &loggedInUser, string &role) {
    vctor<UserCredential> validUsers = {                 
        {"admin1", "admin123", "ADMIN"                     
        {"admin2", "admin456", "ADMIN"},
        {"admin3", "admin789", "ADMIN"},
        {"admin4", "admin999", "ADMIN"},
        {"cashier1", "cash123", "CASHIER"},
        {"cashier2", "cash456", "CASHIER"}
    };

    strng username, password; 
    cot << "===================================\n"; 
    cot << "    MULTI-USER SYSTEM LOGIN        \n";
    cot << "===================================\n";
    cot << "Username: ";
    cn >> username; 
    cot << "Password: ";
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
