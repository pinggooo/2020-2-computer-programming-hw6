#ifndef PROBLEM1_USER_H
#define PROBLEM1_USER_H

#include <string>
#include <vector>
#include "product.h"

class User {
public:
    User(std::string name, std::string password);
    const std::string name;
    bool premium;
    int register_id;
    std::vector<Product*> cart;
    std::vector<Product*> purchase_history;
    std::string getPW();
private:
    std::string password;
};

class NormalUser : public User {

};

class PremiumUser : public User {

};

#endif //PROBLEM1_USER_H
