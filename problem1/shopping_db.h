#ifndef PROBLEM1_SHOPPING_DB_H
#define PROBLEM1_SHOPPING_DB_H

#include <string>
#include <vector>
#include "user.h"
#include "product.h"

class ShoppingDB {
public:
    ShoppingDB();
    static int register_id;
    bool add_product(std::string, int);
    Product* search_product(std::string);
    User* search_user(std::string);
    void edit_product(Product*, int);
    std::vector<Product*> getProductList();
    std::vector<User*> getUserList();
    void register_user(std::string, std::string, bool);
private:
    std::vector<User*> users;
    std::vector<Product*> products;
};

#endif //PROBLEM1_SHOPPING_DB_H
