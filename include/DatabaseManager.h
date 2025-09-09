#pragma once    // so that file gets included only once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <optional>          // its a wrapper, kind of like replacement for pointers
#include "User.h"            // import User class
#include "Post.h"            // import Post class
#include "Enrollments.h"     // import Enrollments class

class DatabaseManager {
private:
    SQLite::Database db;
public:
    // Constructor to open the database connection
    DatabaseManager(const std::string& DbPath);

    // User Related Functions
    bool addUser(const std::string& username, const std::string& password, const std::string& role, const std::string& fullname);
    bool isUsernameTaken(const std::string& username);
    std::optional<User> getUserByUsername(const std::string& username);
    std::string getFullNameByUserID(const int& userID);

    // Post Related Functions
    bool createPost(const Post& post);
    std::vector<Post> getAllPosts();
    std::vector<Post> getPostByOwnerID(int ownerID);
    // Enrollements Related Functions
    // we shall add these when required later.
};