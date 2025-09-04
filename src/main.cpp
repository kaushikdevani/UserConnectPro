#include <iostream>
#include "DatabaseManager.h"

int main() {
    // We will create the DB file in the build directory
    DatabaseManager dbManager("UserConnect.db");

    std::cout << "\n--- Testing User Management ---\n";

    // Test adding a new user
    std::cout << "Attempting to add user 'kaushik'...\n";
    bool success = dbManager.addUser("nithya", "pass123", "Student", "Nithyashree");
    if (success) {
        std::cout << "User 'nithya' added successfully.\n";
    } else {
        std::cout << "Failed to add user 'nithya' (might already exist).\n";
    }

    // Test fetching the user
    std::cout << "\nAttempting to fetch user 'nithya'...\n";
    auto userOptional = dbManager.getUserByUsername("nithya");

    if (userOptional) { // Check if the optional contains a value
        User user = *userOptional; // Get the user object from the optional
        std::cout << "User Found!\n";
        std::cout << "  ID: " << user.id << "\n";
        std::cout << "  Username: " << user.username << "\n";
        std::cout << "  Full Name: " << user.fullname << "\n";
        std::cout << "  Role: " << user.role << "\n";
    } else {
        std::cout << "User 'kaushik' not found.\n";
    }

    return 0;
}