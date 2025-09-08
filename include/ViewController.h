#pragma once     // so that file gets included only once
#include "DatabaseManager.h"
#include <string>
#include <optional>

class ViewController{
private:
    DatabaseManager dbManager;        // for getting the dbManager up running
    std::optional<User> currentUser;  // for storing the current user who's using our application

    void displayMainMenu();
    void handleRegistation();
    void handleLogin();
    void handleCreatePost();
    void displayAllPosts();
    void displayUserPosts();

    //We shall do these later...
    void showStudentDashboard();
    void showTeacherDashboard();

public:
    ViewController(const std::string& dbPath);
    void run();      // this is where the application running takes place... maintains looped UI
    
};