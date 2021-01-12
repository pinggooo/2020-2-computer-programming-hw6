#ifndef PROBLEM2_POST_H
#define PROBLEM2_POST_H
#include <iostream>
#include <ctime>

class Post {
public:
    int id;
    std::string writer;
    std::string dateTime;
    std::string title, content;
    Post(const std::string&, const std::string&);
    Post(const int&, const std::string&, const std::string&, const std::string&);
private:
    std::string timeFormat(const std::string&);
};

#endif //PROBLEM2_POST_H
