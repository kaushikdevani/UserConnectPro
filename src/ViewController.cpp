#include "ViewController.h"
#include <iostream>

ViewController::ViewController(const std::string& dbPath):dbManager(dbPath){
    std::cout << "ViewController Initialized" << std::endl;
}

void ViewController::displayMainMenu(){
    int choice = 0;
    while(choice != 3){
        std::cout << "\n===== Welcome to User-Connect =====\n";
        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "3. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice)
        {
            case 1:
                handleLogin();
                break;
            case 2:
                handleRegistation();
                break;
            case 3:
                std::cout << "Exiting!" << std::endl;
                std::cout << "Sayonara!!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cin.clear();               // Clear the error flag
                std::cin.ignore(1000, '\n');    // Discard the bad input
                break;
        }

        if(currentUser){
            break;
            // If a user has successfully logged in, it goes to dashboard, 
            // after dashboard exists we dont want the program to display the Main Menu again.
        }

    }
}

void ViewController::handleRegistation(){
    std::string username, password, fullname, role;
    int roleNum = 0;

    std::cout << "\n--- New User Registration ---\n";
    std::cout << "Enter username: ";
    std::cin  >> username;
    std::cout << "Enter password: ";
    std::cin  >> password;
    std::cout << "Enter full name: ";
    std::cin  >> fullname;
    std::cout << "Register as (1) Student or (2) Teacher" << std::endl;
    std::cout << "Enter the corresponding number : ";
    std::cin >> roleNum;


    while (std::cin.fail()) {
        std::cin.clear();               // Clear the error flag
        std::cin.ignore(1000, '\n');    // Discard the bad input

        std::cout << "Invalid input! Not an integer. Please select from 1 or 2" << std::endl;
        std::cin >> roleNum;
    }

    if(roleNum == 1){
        role = "Teacher";
    }else if(roleNum == 2){
        role = "Student";
    }else{
        std::cout << "The input must be either 1 or 2" << std::endl;
    }

    bool success = dbManager.addUser(username,password,role,fullname);

    if (success) {
        std::cout << "Registration successful! You can now log in." << std::endl;
    } else {
        std::cout << "Registration failed. That username might already be taken. Please Retry." << std::endl;
    }
}

void ViewController::handleLogin(){
    std::string username, password;

    std::cout << "\n--- User Login ---\n";
    std::cout << "Enter username: ";
    std::cin >> username;
    std::cout << "Enter password: ";
    std::cin >> password;

    // Temperaray code -- must update with actual hashpassword security
    currentUser = dbManager.getUserByUsername(username);

    if(currentUser){
        std::cout << "Login successful! Welcome, " << currentUser->fullname << "." << std::endl;;
    }else{
        std::cout << "Login failed. Invalid username or password." << std::endl;
    }
}

void ViewController::showStudentDashboard() {
    std::cout << "\n--- Welcome to the Student Dashboard (Work in Progress) ---\n";
}

void ViewController::showTeacherDashboard() {
    std::cout << "\n--- Welcome to the Teacher Dashboard (Work in Progress) ---\n";
}

void ViewController::run(){
    displayMainMenu();

    if(currentUser){
        if(currentUser->role == "Student"){
            showStudentDashboard();
        }else if(currentUser->role == "Teacher"){
            showTeacherDashboard();
        }
    }
}