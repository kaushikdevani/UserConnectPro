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
    std::cin.ignore();
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    std::cout << "Enter full name: ";
    std::getline(std::cin, fullname);
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
    std::cin.ignore();
    std::cout << "Enter username: ";
    std::getline(std::cin, username);
    std::cout << "Enter password: ";
    std::getline(std::cin, password);

    // Temperaray code -- must update with actual hashpassword security
    currentUser = dbManager.getUserByUsername(username);

    if(currentUser){
        std::cout << "Login successful! Welcome, " << currentUser->fullname << "." << std::endl;;
    }else{
        std::cout << "Login failed. Invalid username or password." << std::endl;
    }
}

void ViewController::handleCreatePost(){
    Post post;
    int typeNum;
    std::cout << "\n--- Create New Post ---\n";
    std::cout << "Select type: (1)Course (2)Project.\nPlease enter the corresponding number : " << std::endl;
    std::cin >> typeNum;
    while (std::cin.fail()) {
        std::cin.clear();               // Clear the error flag
        std::cin.ignore(1000, '\n');    // Discard the bad input

        std::cout << "Invalid input! Not an integer. Please select from 1 or 2" << std::endl;
        std::cin >> typeNum;
    }
    if(typeNum == 1){
        post.type = "Course";
    }else if(typeNum == 2){
        post.type = "Project";
    }else{
        std::cout << "The input must be either 1 or 2" << std::endl;
    }
    std::cin.ignore();
    std::cout << "Enter title: " << std::endl;
    std::getline(std::cin, post.title);

    std::cout << "Enter Description: " << std::endl;
    std::getline(std::cin,post.description);

    std::cout << "Enter Tags (e.g #c++ #oops #boring class): ";
    std::getline(std::cin, post.tags);

    bool success = dbManager.createPost(post);

    if(success){
        std::cout << "Post Created Successfully." << std::endl;
    }else{
        std::cout << "Failed to create Post." << std::endl;
    }
    
}

void ViewController::displayAllPosts(){
    std::vector<Post> posts = dbManager.getAllPosts();
    int total_posts = posts.size();

    if(total_posts == 0){
        std::cout << "\nNo Posts To Display.\n" << std::endl;
        return;
    }
    std::cout << "------------------Posts------------------" << std::endl;
    for(int i=0; i<total_posts; i++){
        std::cout << "----------------------------------------\n";
        std::cout << "ID: " << posts[i].id << " | Type: " << posts[i].type << " | Posted By: " << dbManager.getUserNameByUserID(posts[i].ownerID)<< std::endl;
        std::cout << "Title: " << posts[i].title << std::endl;
        std::cout << "Description: " << posts[i].description << std::endl;
        std::cout << "Tags: " << posts[i].tags << std::endl;
        std::cout << "----------------------------------------\n";
    }
    std::cout << "No more Posts to show !" << std::endl;
}

void ViewController::displayUserPosts(){
    std::vector<Post> posts = dbManager.getPostByOwnerID(currentUser->id);
    int total_posts = posts.size();

    if(total_posts == 0){
        std::cout << "\nYou have No Posts To Display.\n" << std::endl;
        return;
    }
    std::cout << "------------- Your Posts -------------" << std::endl;
    for(int i=0; i<total_posts; i++){
        std::cout << "----------------------------------------\n";
        std::cout << "ID: " << posts[i].id << " | Type: " << posts[i].type << " | Posted By: " << currentUser->fullname<< std::endl;
        std::cout << "Title: " << posts[i].title << std::endl;
        std::cout << "Description: " << posts[i].description << std::endl;
        std::cout << "Tags: " << posts[i].tags << std::endl;
        std::cout << "----------------------------------------\n";
    }
    std::cout << "No more Posts to show !" << std::endl;
}

void ViewController::showStudentDashboard() {
    std::cout << "\n--- Welcome to the Student Dashboard ---\n";

    int choice = 0;

    while(choice != 4){
        std::cout << "\n--- Welcome , " << currentUser->fullname << " , what do want to do today?---\n";
        std::cout << "1. View all Courses, Projects and Posts" << std::endl;
        std::cout << "2. Show My Courses, Projects and Posts" << std::endl;
        std::cout << "3. Create a new Post" << std::endl;
        std::cout << "4. Logout" << std::endl;
        std::cout << "Enter your choice: " << std::endl;
        std::cin >> choice;
    
        switch (choice) {
            case 1: {
                displayAllPosts();
                break;
            }
            case 2: {
                displayUserPosts();
                break;
            }
            case 3: {
                handleCreatePost();
                break;
            }
            case 4:
                std::cout << "Logging out...\n";
                currentUser = std::nullopt;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }
}

void ViewController::showTeacherDashboard() {
    std::cout << "\n--- Welcome to the Teacher Dashboard ---\n";

    int choice = 0;
    std::cout << "\n--- Welcome Prof. " << currentUser->fullname << " , what do want to do today?---\n";

    while(choice != 4){
        std::cout << "1. View all Courses, Projects and Projects" << std::endl;
        std::cout << "2. Show My Courses, Projects and Posts" << std::endl;
        std::cout << "3. Create a new Post" << std::endl;
        std::cout << "4. Logout" << std::endl;
        std::cout << "Enter your choice: " << std::endl;
        std::cin >> choice;
    
        switch (choice) {
            case 1: {
                displayAllPosts();
                break;
            }
            case 2: {
                displayUserPosts();
                break;
            }
            case 3: {
                handleCreatePost();
                break;
            }
            case 4:
                std::cout << "Logging out...\n";
                currentUser = std::nullopt;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }
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