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
    if(currentUser){
        if(currentUser->role == "Student"){
            showStudentDashboard();
        }else if(currentUser->role == "Teacher"){
            showTeacherDashboard();
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
        role = "Student";
    }else if(roleNum == 2){
        role = "Teacher";
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

    if(dbManager.verifyLogin(username,password)){
        currentUser = dbManager.getUserByUsername(username);
        std::cout << "Login successful! Welcome, " << currentUser->fullname << "." << std::endl;;
    }else{
        std::cout << "Invalid Username/Password\n" << std::endl;
    }
}

void ViewController::handleCreatePost(){
    Post post;
    int typeNum;
    std::cout << "\n--- Create New Course/Project ---\n";
    std::cout << "Select type: (1)Course (2)Project.\nPlease enter the corresponding number : " << std::endl;
    std::cin >> typeNum;
    while (std::cin.fail()) {
        std::cin.clear();               // Clear the error flag
        std::cin.ignore(1000, '\n');    // Discard the bad input

        std::cout << "Invalid input! Not an integer. Please select from 1 or 2. \n" << std::endl;
        return;
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

    post.ownerID = currentUser->id;

    bool success = dbManager.createPost(post);

    if(success){
        std::cout << "Course/Project Created Successfully.\n" << std::endl;
    }else{
        std::cout << "Failed to create Post.\n" << std::endl;
    }
    
}

void ViewController::handleApplyPost(){
    std::cout << "\n--- Apply to a Post ---\n";
    std::cout << "Enter the ID of the post you want to apply for: ";
    int postId;
    std::cin >> postId;

    // Check if the input failed (e.g., user entered "abc" instead of a number)
    if (std::cin.fail()) {
        std::cin.clear(); // 1. Reset the error flag on cin
        std::cin.ignore(10000, '\n'); // 2. Flush the bad input from the stream
        std::cout << "Invalid input. Please enter a valid Post ID number.\n";
        return; // 3. Exit the function early
    }
    
    if (dbManager.applyToPost(currentUser->id, postId)) {
        std::cout << "Application successful! The teacher will be notified.\n";
    } else {
        std::cout << "Application failed. You may have already applied or the post ID is invalid.\n";
    }
}

void ViewController::handleApplicants(){
    std::cout << "\n--- All Applications ---\n";
    std::vector<Enrollments> applications = dbManager.getApplicationsForUser(currentUser->id);
    if (applications.empty()) {
        std::cout << "You have no pending applications at this time.\n" << std::endl;
        return;
    }

    // 1. Display the numbered list of all applications
    std::cout << "Here are all pending applications for your posts:\n";
    for (size_t i = 0; i < applications.size(); ++i) {
        std::cout << "-----------------------------------\n";
        std::cout << "Row " << (i + 1) << ": \n";
        std::cout << "  Post     :" << applications[i].postID << "\n";
        std::cout << "  Student  :" << applications[i].userID << "\n";
        std::cout << "  Status   :" << applications[i].status << "\n";
    }
    std::cout << "-----------------------------------\n\n";

    // 2. Ask what action the teacher wants to take
    std::cout << "What action would you like to take?\n";
    std::cout << "1. Approve an Application\n";
    std::cout << "2. Reject an Application\n";
    std::cout << "3. Go Back (Do Nothing)\n";
    std::cout << "Enter your choice: ";
    
    int choice;
    std::cin >> choice;

    // Check if the input failed (e.g., user entered "abc" instead of a number)
    if (std::cin.fail()) {
        std::cin.clear(); // 1. Reset the error flag on cin
        std::cin.ignore(10000, '\n'); // 2. Flush the bad input from the stream
        std::cout << "Invalid input. Please enter a valid Post ID number.\n";
        return; // 3. Exit the function early
    }

    // Handle "Go Back"
    if (choice != 1 && choice != 2) {
        std::cout << "Returning to Teacher Dashboard...\n\n";
        return;
    }

    // 3. Get the row number to process
    std::cout << "Enter the Row Number of the Application to process: ";
    int rowNum;
    std::cin >> rowNum;

    // Validate the row number
    if (rowNum < 1 || rowNum > applications.size()) {
        std::cout << "Invalid row number. Returning to dashboard.\n\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }

    // Get the details from the selected row (0-indexed)
    const auto& selectedApp = applications[rowNum - 1];

    // 4. Perform the chosen action
    if (choice == 1) { // Approve
        if (dbManager.approveApplication(selectedApp.userID, selectedApp.postID)) {
            std::cout << "Success! " << selectedApp.userID << " has been approved.\n";
        } else {
            std::cout << "Failed to approve application.\n";
        }
    } else if (choice == 2) { // Reject
        if (dbManager.rejectApplication(selectedApp.userID, selectedApp.postID)) {
            std::cout << "Success! " << selectedApp.userID << "'s application has been rejected.\n";
        } else {
            std::cout << "Failed to reject application.\n";
        }
    }

    std::cout << "Returning to Teacher Dashboard...\n\n";
}

void ViewController::displayAllPosts(){
    std::vector<Post> posts = dbManager.getAllPosts();
    int total_posts = posts.size();

    if(total_posts == 0){
        std::cout << "\nNo Courses/Projects To Display.\n" << std::endl;
        return;
    }
    std::cout << "--- Courses and Projects ---" << std::endl;
    for(int i=0; i<total_posts; i++){
        std::cout << "----------------------------------------\n";
        std::cout << "ID: " << posts[i].id << " | Type: " << posts[i].type << " | Posted By: " << dbManager.getFullNameByUserID(posts[i].ownerID)<< std::endl;
        std::cout << "Title: " << posts[i].title << std::endl;
        std::cout << "Description: " << posts[i].description << std::endl;
        std::cout << "Tags: " << posts[i].tags << std::endl;
        std::cout << "----------------------------------------\n";
    }
    std::cout << "No more Posts to show !" << std::endl;
}

void ViewController::displayUserPosts(){
    
    std::vector<Post> posts;
    if(currentUser->role == "Teacher"){
        posts = dbManager.getPostByOwnerID(currentUser->id);
    }else if(currentUser->role == "Student"){
        posts = dbManager.getMyPosts(currentUser->id);
    }
    int total_posts = posts.size();

    if(total_posts == 0){
        std::cout << "\nYou have No Courses/Projects To Display.\n" << std::endl;
        return;
    }
    std::cout << "------------- Your Courses/Projects -------------" << std::endl;
    for(int i=0; i<total_posts; i++){
        std::cout << "----------------------------------------\n";
        std::cout << "ID: " << posts[i].id << " | Type: " << posts[i].type << " | Posted By: " << dbManager.getFullNameByUserID(posts[i].ownerID)<< std::endl;
        std::cout << "Title: " << posts[i].title << std::endl;
        std::cout << "Description: " << posts[i].description << std::endl;
        std::cout << "Tags: " << posts[i].tags << std::endl;
        std::cout << "----------------------------------------\n";
    }
}

void ViewController::displayAllTweets(){
    std::cout << "\n--- ALl Tweets Feed ---\n";
    std::vector<Tweets> tweets = dbManager.getAllTweets();

    if (tweets.empty()) {
        std::cout << "The feed is empty. Be the first to post!\n";
        return;
    }

    for (const Tweets& tweet : tweets) {
        std::cout << "-----------------------------------\n";
        std::cout << "From: " << tweet.owner_name << " (User ID: " << tweet.owner_id << ")\n";
        std::cout << "At:   " << tweet.time_stamp << "\n";
        std::cout << "\n  " << tweet.content << "\n";
    }
    std::cout << "-----------------------------------\n";
}

void ViewController::displayMyTweets(){
    std::cout << "\n--- My Tweets Feed ---\n";
    std::vector<Tweets> mytweets = dbManager.getMyTweets(currentUser->id);

    if (mytweets.empty()) {
        std::cout << "You did not create any tweets. Create your first tweet!\n" << std::endl;
        return;
    }

    for (const Tweets& tweet : mytweets) {
        std::cout << "-----------------------------------\n";
        std::cout << "From: " << tweet.owner_name << " (User ID: " << tweet.owner_id << ")\n";
        std::cout << "At:   " << tweet.time_stamp << "\n";
        std::cout << "\n  " << tweet.content << "\n";
    }
    std::cout << "-----------------------------------\n";
}

void ViewController::handleCreateTweet(){
    std::string content;
    std::cout << "\n--- Create a New Tweet ---\n";
    std::cout << "Enter your message: ";

    std::cin.ignore(10000, '\n'); // Clear any leftover input
    std::getline(std::cin, content);

    if (content.empty()) {
        std::cout << "Tweet cannot be empty.\n";
        return;
    }

    if (dbManager.createTweet(currentUser->id, content)) {
        std::cout << "Tweet posted successfully!\n";
    } else {
        std::cout << "Failed to post tweet.\n";
    }
    std::cout << std::endl;
}

void ViewController::showStudentDashboard() {
    std::cout << "\n--- Welcome to the Student Dashboard ---\n";

    int choice = 0;

    while(choice != 7){
        std::cout << "\n--- Welcome , " << currentUser->fullname << " , what do want to do today?---\n";
        std::cout << "1. View all Courses and Projects" << std::endl;
        std::cout << "2. Show My Courses and Projects" << std::endl;
        std::cout << "3. Apply to a Course/Project" << std::endl;
        std::cout << "4. All Tweets" << std::endl;
        std::cout << "5. My Tweets" << std::endl;
        std::cout << "6. Create Tweet" << std::endl;
        std::cout << "7. Logout" << std::endl;
        std::cout << "Enter your choice: " << std::endl;
        std::cin >> choice;
    
        switch (choice) {
            case 1:
                displayAllPosts();
                break;
            case 2:
                displayUserPosts();
                break;
            case 3:
                handleApplyPost();
                break;
            case 4:
                displayAllTweets();
                break;
            case 5:
                displayMyTweets();
                break;
            case 6:
                handleCreateTweet();
                break;
            case 7:
                std::cout << "Logging out...\n";
                currentUser = std::nullopt;
                break;
            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }
    displayMainMenu();
}

void ViewController::showTeacherDashboard() {
    std::cout << "\n--- Welcome to the Teacher Dashboard ---\n";

    int choice = 0;
    std::cout << "\n--- Welcome Prof. " << currentUser->fullname << " , what do want to do today?---\n";

    while(choice != 8){
        std::cout << "1. View all Courses and Projects" << std::endl;
        std::cout << "2. Show My Courses and Projects" << std::endl;
        std::cout << "3. Create a new Course/Project" << std::endl;
        std::cout << "4. View Applicants for My Courses/Projects" << std::endl;
        std::cout << "5. All Tweets" << std::endl;
        std::cout << "6. My Tweets" << std::endl;
        std::cout << "7. Create Tweet" << std::endl;
        std::cout << "8. Logout" << std::endl;
        std::cout << "Enter your choice: " << std::endl;
        std::cin >> choice;
    
        switch (choice) {
            case 1:
                displayAllPosts();
                break;
            case 2:
                displayUserPosts();
                break;
            case 3:
                handleCreatePost();
                break;
            case 4:
                handleApplicants();
                break;
            case 5:
                displayAllTweets();
                break;
            case 6:
                displayMyTweets();
                break;
            case 7:
                handleCreateTweet();
                break;
            case 8:
                std::cout << "Logging out...\n";
                currentUser = std::nullopt;
                break;

            default:
                std::cout << "Invalid choice. Please try again.\n";
                std::cin.clear();
                std::cin.ignore(10000, '\n');
        }
    }
    displayMainMenu();
}

void ViewController::run(){
    displayMainMenu();

    
}