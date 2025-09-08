#include "DatabaseManager.h"
#include <iostream>

// there is no default constructor for SQLite::Database , so we have to construct it in the member initializer list

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

std::string DatabaseManager::getUserNameByUserID(const int& userID){
    try{
        SQLite::Statement query(db, R"(
                SELECT username
                FROM Users
                WHERE id = ?
            )");
        query.bind(1,userID);

        if(query.executeStep()){
            return query.getColumn("username").getString();
        }
    }catch(const std::exception& e){
        std::cout << "Database error at getUserNameByUserID." << std::endl;
    }
    return "user unknown/ doesn't exist";     // if there is error, we will inform this as the output of the string.
}

bool DatabaseManager::createPost(const Post& post){
    try{
        SQLite::Statement query(db, "INSERT INTO Posts (type,title,description,owner_id,tags) VALUES (?,?,?,?,?)");
        query.bind(1,post.type);
        query.bind(2,post.title);
        query.bind(3,post.description);
        query.bind(4,post.ownerID);
        query.bind(5,post.tags);
        query.exec();
        return true;
    }catch(const std::exception& e){
        std::cerr << "Database error in CreatePost: " << e.what() << std::endl;
        return false;
    }
}

std::vector<Post> DatabaseManager::getAllPosts(){
    std::vector<Post> posts;
    try{
        SQLite::Statement query(db,"SELECT * FROM Posts");
        while(query.executeStep()){
            Post p;
            p.id = query.getColumn("id");
            p.title = query.getColumn("title").getString();
            p.type = query.getColumn("type").getString();
            p.description = query.getColumn("description").getString();
            p.tags = query.getColumn("tags").getString();
            p.ownerID = query.getColumn("owner_id").getInt();
            posts.push_back(p);
        }
    }catch(const std::exception& e){
        std::cout << "Database Error at getAllPosts: " << e.what() << std::endl;
    }
    return posts;
}

std::vector<Post> DatabaseManager::getPostByOwnerID(int owner_id){
    std::vector<Post> posts;
    try{
        SQLite::Statement query(db,"SELECT * FROM Posts WHERE owner_id = ?");
        query.bind(1,owner_id);
        while(query.executeStep()){
            Post p;
            p.id = query.getColumn("id");
            p.title = query.getColumn("title").getString();
            p.type = query.getColumn("type").getString();
            p.description = query.getColumn("description").getString();
            p.tags = query.getColumn("tags").getString();
            p.ownerID = query.getColumn("owner_id").getInt();
            posts.push_back(p);
        }
    }catch(const std::exception& e){
        std::cout << "Database Error at getPostByOwnerID: " << e.what() << std::endl;
    }
    return posts;
}