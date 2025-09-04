#pragma once // for making sure that file includes only once
#include <string>

class Post{
public:
    int id;
    std::string type;
    std::string title;
    std::string description;
    int ownerID;
    std::string tags;     //tags could be helpul for searching 
};