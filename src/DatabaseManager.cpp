#include "DatabaseManager.h"
#include <iostream>

// there isnt a default constructor for SQLite::Database , so we have to construct it in the member initializer list

DatabaseManager::DatabaseManager(const std::string& dbPath)
    :db(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE){
    // a simple output to confirm database is opened
    std::cout << "DatabaseManager initialized with database: " << db.getFilename() << std::endl;

    // if the database schema is not created yet, we need to create it

    try{
        // fun fact we can use R"(any random text with any character can be used) is called a raw string literal which is handy for SQL queries
        db.exec(R"(
            CREATE TABLE IF NOT EXISTS Users (
                id            INTEGER PRIMARY KEY AUTOINCREMENT,
                username      TEXT NOT NULL UNIQUE,
                password_hash TEXT NOT NULL,
                role          TEXT NOT NULL CHECK(role IN ('Student', 'Teacher')),
                full_name     TEXT NOT NULL,
                bio           TEXT,
                skills        TEXT
            );
        )");

        db.exec(R"(
            CREATE TABLE IF NOT EXISTS Posts (
                id            INTEGER PRIMARY KEY AUTOINCREMENT,
                type          TEXT NOT NULL CHECK(type IN ('Course', 'Project')),
                title         TEXT NOT NULL,
                description   TEXT,
                owner_id      INTEGER NOT NULL,
                tags          TEXT,
                FOREIGN KEY (owner_id) REFERENCES Users (id)
            );
        )");

        db.exec(R"(
            CREATE TABLE IF NOT EXISTS Enrollments (
                user_id       INTEGER NOT NULL,
                post_id       INTEGER NOT NULL,
                status        TEXT NOT NULL CHECK(status IN ('Enrolled', 'Applied', 'Member')),
                PRIMARY KEY (user_id, post_id),
                FOREIGN KEY (user_id) REFERENCES Users (id),
                FOREIGN KEY (post_id) REFERENCES Posts (id)
            );
        )");
        std::cout << "Database schema verified/created successfully." << std::endl;

    }catch(const std::exception& e){
        std::cerr << "Error creating schema: " << e.what() << std::endl;
    }
}

// write the rest of the member function executions here

bool DatabaseManager::addUser(const std::string& username, const std::string& password, const std::string& role, const std::string& fullname){
    if(isUsernameTaken(username)){
        return false;
    }
    try {
        SQLite::Statement query(db, "INSERT INTO Users (username, password_hash, role, full_name) VALUES (?, ?, ?, ?)");
        
        // Bind parameters to the statement
        // Note: We are not hashing the password yet, we will add that later.
        query.bind(1, username);
        query.bind(2, password); // Placeholder for password_hash
        query.bind(3, role);
        query.bind(4, fullname);
        
        // Execute the query
        query.exec();
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Database error in addUser: " << e.what() << std::endl;
        return false;
    }
}

bool DatabaseManager::isUsernameTaken(const std::string& username){
    try{
        SQLite::Statement query(db, R"(
                SELECT * 
                FROM Users 
                WHERE username = ?
            )" );

        // Bind parameters to the statement
        query.bind(1, username);

        // `executeStep()` returns true if a row was found
        return query.executeStep();

    } catch (const std::exception& e){
        std::cerr << "Database error in isUsernameTaken: " << e.what() << std::endl;
        return true;  // Assume taken on error to be safe
    }
}

std::optional<User> DatabaseManager::getUserByUsername(const std::string& username){
    try{
        SQLite::Statement query(db, R"(
                SELECT *
                FROM Users
                WHERE username = ?
            )");

        // Bind parameters to the statement
        query.bind(1, username);

        // if user is found
        if(query.executeStep()) {
            User user;
            user.id = query.getColumn("id");
            user.username = query.getColumn("username").getString();
            user.fullname = query.getColumn("full_name").getString();
            user.role = query.getColumn("role").getString();
            user.bio = query.getColumn("bio").getString();
            user.skills = query.getColumn("skills").getString();
            return user;
        }

    } catch (const std::exception& e){
        std::cerr << "Database error in getUserByUsername: " << e.what() << std::endl;
    }
    // return empty optional (empty wrapper) if not found/ error
    return std::nullopt;
}