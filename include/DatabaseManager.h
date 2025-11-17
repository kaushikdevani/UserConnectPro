#pragma once    // so that file gets included only once

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <optional>          // its a wrapper, kind of like replacement for pointers
#include "User.h"            // import User class
#include "Post.h"            // import Post class
#include "Enrollments.h"     // import Enrollments class
#include "Tweets.h"          // import Tweets class

class DatabaseManager {
private:
    SQLite::Database db;

    //Private hash function for string Password
    std::string hashPassword(const std::string& password);
public:
    // Constructor to open the database connection
    DatabaseManager(const std::string& DbPath);

    // User Related Functions
    bool addUser(const std::string& username, const std::string& password, const std::string& role, const std::string& fullname);
    bool verifyLogin(const std::string& given_username, const std::string given_password);
    bool isUsernameTaken(const std::string& username);
    std::optional<User> getUserByUsername(const std::string& username);
    std::string getFullNameByUserID(const int& userID);
    Post getPostFromPostID(int postID);

    // Post Related Functions
    bool createPost(const Post& post);
    std::vector<Post> getAllPosts();
    std::vector<Post> getPostByOwnerID(int ownerID);
    std::vector<Post> getMyPosts(int ownerID);

    // Enrollements Related Functions
    bool applyToPost(int userID, int postID);
    std::vector<Enrollments> getApplicationsForPost(int postID);
    std::vector<Enrollments> getApplicationsForUser(int userID);
    bool approveApplication(int userID, int postID);
    bool rejectApplication(int userID, int postID);

    // Tweets Related Functions
    bool createTweet(int userID,const std::string& content);
    std::vector<Tweets> getAllTweets();
    std::vector<Tweets> getMyTweets(int userID);
};