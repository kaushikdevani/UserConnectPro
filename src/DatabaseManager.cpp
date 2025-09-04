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
        
    }catch(const std::exception& e){
        std::cerr << "Error creating schema: " << e.what() << std::endl;
    }
}

// write the rest of the member function executions here

bool DatabaseManager::addUser(const std::string& username, const std::string& password, const std::string& role, const std::string& fullname){

}