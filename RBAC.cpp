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

    for (const auto& user : validUsers) {
        if (user.username == username && user.password == password) {
            loggedInUser = user.username;
            role = user.role;

            time_t now = time(nullptr);
            char timestamp[100];
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

          
            ofstream auditFile("audit_trail.log", ios::app);
            if (auditFile.is_open()) {
                auditFile << "[" << timestamp << "] User: " << loggedInUser << " | Role: " << role << " | Action: USER_LOGIN | Details: Successful authentication\n";
                auditFile.close();
            }

            ofstream logFile("login_logs.txt", ios::app);
            if (logFile.is_open()) {
                logFile << "User: " << loggedInUser << " | Role: " << role << " | Login Time: " << timestamp << "\n";
                logFile.close();
            }

            cout << "\n[Access Granted] Welcome, " << loggedInUser << " (" << role << " mode logged at " << timestamp << ").\n";
            return true;
        }
    }

    
    time_t now = time(nullptr);
    char timestamp[100];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));
    ofstream auditFile("audit_trail.log", ios::app);
    if (auditFile.is_open()) {
        auditFile << "[" << timestamp << "] User: UNKNOWN | Role: UNAUTHENTICATED | Action: SECURITY_ALERT | Details: Failed login attempt with username: " << username << "\n";
        auditFile.close();
    }

    cout << "\n[Access Denied] Invalid username or password.\n";
    return false;
}
