#pragma once   // so that the file only include once (safety reasons)

#include <string>

class User{
public:
    int id;
    std::string username;
    std::string fullname;
    std::string role;
    std::string bio;
    std::string skills;
};