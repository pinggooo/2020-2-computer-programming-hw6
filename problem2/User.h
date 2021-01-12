#ifndef PROBLEM2_USER_H
#define PROBLEM2_USER_H

#include <iostream>

class User {
public:
    User(const std::string&, const std::string&);
    std::string id;
    std::string getPW();
private:
    std::string password;
};

#endif //PROBLEM2_USER_H
