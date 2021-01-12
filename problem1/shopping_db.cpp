#include "shopping_db.h"

int ShoppingDB::register_id = 0;

ShoppingDB::ShoppingDB() {
}

bool ShoppingDB::add_product(std::string name, int price) {
    if (price > 0) {
        products.push_back(new Product(name, price));
        return true;
    } else {
        return false;
    }
}

void ShoppingDB::edit_product(Product* product, int price) {
    product->price = price;
}

Product* ShoppingDB::search_product(std::string name) {
    for (auto iter = products.begin(); iter != products.end(); iter++) {
        if ((*iter)->name == name) {
            return *iter;
        }
    }

    return nullptr;
}

std::vector<Product*> ShoppingDB::getProductList() {
    return products;
}

std::vector<User *> ShoppingDB::getUserList() {
    return users;
}

void ShoppingDB::register_user(std::string username, std::string password, bool premium) {
    User* user = new User(username, password);
    user->premium = premium;
    user->register_id = register_id++;
    users.push_back(user);
}

User* ShoppingDB::search_user(std::string name) {
    for (auto iter = users.begin(); iter != users.end(); iter++) {
        if((*iter)->name == name) {
            return *iter;
        }
    }

    return nullptr;
}

