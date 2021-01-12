#include <vector>
#include "client_ui.h"
#include "product.h"
#include "user.h"

ClientUI::ClientUI(ShoppingDB &db, std::ostream& os) : UI(db, os), current_user() { }

ClientUI::~ClientUI() {
    delete current_user;
}

void ClientUI::signup(std::string username, std::string password, bool premium) {
    // TODO: For problem 1-2
    db.register_user(username, password, premium);
    os << "CLIENT_UI: " << username << " is signed up." << std::endl;
}

void ClientUI::login(std::string username, std::string password) {
    // TODO: For problem 1-2
    User* searched = db.search_user(username);
    if (current_user != nullptr) {
        os << "CLIENT_UI: Please logout first." << std::endl;
    } else if (searched == nullptr) {
        os << "CLIENT_UI: Invalid username or password." << std::endl;
    } else if (searched->getPW() != password) {
        os << "CLIENT_UI: Invalid username or password." << std::endl;
    } else {
        current_user = searched;
        os << "CLIENT_UI: " << username << " is logged in." << std::endl;
    }
}

void ClientUI::logout() {
    // TODO: For problem 1-2
    if (current_user != nullptr) {
        os << "CLIENT_UI: " << current_user->name << " is logged out." << std::endl;
        current_user = nullptr;
    } else {
        os << "CLIENT_UI: There is no logged-in user." << std::endl;
    }
}

void ClientUI::add_to_cart(std::string product_name) {
    // TODO: For problem 1-2
    if (current_user == nullptr) {
        os << "CLIENT_UI: Please login first." << std::endl;
        return;
    }

    Product* product = db.search_product(product_name);
    if (product == nullptr) {
        os << "CLIENT_UI: Invalid product name." << std::endl;
    } else {
        current_user->cart.push_back(product);
        os << "CLIENT_UI: " << product->name << " is added to the cart." << std::endl;
    }
}

void ClientUI::list_cart_products() {
    // TODO: For problem 1-2.
    if (current_user == nullptr) {
        os << "CLIENT_UI: Please login first." << std::endl;
        return;
    }

    os << "CLIENT_UI: Cart: [";
    int idx = 0;
    for (auto iter = current_user->cart.begin(); iter != current_user->cart.end(); iter++) {
        if (current_user->premium) {
            if (idx++ == current_user->cart.size() - 1) {
                os << "(" << (*iter)->name << ", " << premium_price((*iter)->price) << ")";
            } else {
                os << "(" << (*iter)->name << ", " << premium_price((*iter)->price) << "), ";
            }
        } else {
            if (idx++ == current_user->cart.size() - 1) {
                os << "(" << (*iter)->name << ", " << (*iter)->price << ")";
            } else {
                os << "(" << (*iter)->name << ", " << (*iter)->price << "), ";
            }
        }
    }

    os << "]" << std::endl;
}

void ClientUI::buy_all_in_cart() {
    // TODO: For problem 1-2
    if (current_user == nullptr) {
        os << "CLIENT_UI: Please login first." << std::endl;
        return;
    }

    int total_price = 0;
    for (auto iter = current_user->cart.begin(); iter != current_user->cart.end(); iter++) {
        if (current_user->premium) {
            total_price += premium_price((*iter)->price);
            current_user->purchase_history.push_back(*iter);
        } else {
            total_price += (*iter)->price;
            current_user->purchase_history.push_back(*iter);
        }
    }
    current_user->cart.clear();
    os << "CLIENT_UI: Cart purchase completed. Total price: " << total_price << "." << std::endl;
}

void ClientUI::buy(std::string product_name) {
    // TODO: For problem 1-2
    if (current_user == nullptr) {
        os << "CLIENT_UI: Please login first." << std::endl;
        return;
    }

    Product* product = db.search_product(product_name);
    if (product == nullptr) {
        os << "CLIENT_UI: Invalid product name." << std::endl;
    } else if (current_user->premium) {
        current_user->purchase_history.push_back(product);
        os << "CLIENT_UI: Purchase completed. Price: " << premium_price(product->price) << "." << std::endl;
    } else {
        current_user->purchase_history.push_back(product);
        os << "CLIENT_UI: Purchase completed. Price: " << product->price << "." << std::endl;
    }
}

void ClientUI::recommend_products() {
    // TODO: For problem 1-3.
    if (current_user == nullptr) {
        os << "CLIENT_UI: Please login first." << std::endl;
        return;
    }

    os << "CLIENT_UI: Recommended products: [";

    std::vector<Product*> purchase_history = current_user->purchase_history;
    std::vector<Product*> recommended;
    int idx = 0;
    if (current_user->premium) {
        std::vector<std::pair<User*, int>> userSimilarity;
        std::vector<User*> userList = db.getUserList();
        for (auto iter = userList.begin(); iter != userList.end(); iter++) {
            if (*iter == current_user) continue;
            userSimilarity.emplace_back(*iter, similarity(*iter));
        }
        std::sort(userSimilarity.begin(), userSimilarity.end(), compare);

        for (auto iter = userSimilarity.begin(); iter != userSimilarity.end(); iter++) {
            std::vector<Product*> user_history = (*iter).first->purchase_history;
            if (user_history.empty()) continue;
            if (idx == 2 || iter == userSimilarity.end() - 1) {
                if (std::find(recommended.begin(), recommended.end(), user_history[user_history.size() - 1]) != recommended.end()) {
                    continue;
                }
                recommended.push_back(user_history[user_history.size() - 1]);
                break;
            } else {
                if (std::find(recommended.begin(), recommended.end(), user_history[user_history.size() - 1]) != recommended.end()) {
                    continue;
                }
                recommended.push_back(user_history[user_history.size() - 1]);
                idx++;
            }
        }
    } else {
        for (auto iter = purchase_history.rbegin(); iter != purchase_history.rend(); iter++) {
            if (idx == 2 || iter == purchase_history.rend() - 1) {
                if (std::find(recommended.begin(), recommended.end(), *iter) != recommended.end()) {
                    continue;
                }
                recommended.push_back(*iter);
                break;
            } else {
                if (std::find(recommended.begin(), recommended.end(), *iter) != recommended.end()) {
                    continue;
                }
                recommended.push_back(*iter);
                idx++;
            }
        }
    }

    for (auto iter = recommended.begin(); iter != recommended.end(); iter++) {
        if (current_user->premium) {
            if (iter == recommended.end() - 1) {
                os << "(" << (*iter)->name << ", " << premium_price((*iter)->price) << ")";
            } else {
                os << "(" << (*iter)->name << ", " << premium_price((*iter)->price) << "), ";
            }
        } else {
            if (iter == recommended.end() - 1) {
                os << "(" << (*iter)->name << ", " << (*iter)->price << ")";
            } else {
                os << "(" << (*iter)->name << ", " << (*iter)->price << "), ";
            }
        }
    }

    os << "]" << std::endl;
}

int ClientUI::premium_price(int price) {
    double premiumPrice = price * 0.9;
    if (premiumPrice - int(premiumPrice) < 0.5) {
        return int(premiumPrice);
    } else {
        return int(premiumPrice + 1);
    }
}

int ClientUI::similarity(User* user) {
    std::unordered_set<Product*> currentUserHis;
    std::unordered_set<Product*> userHis;
    int similarity = 0;

    for (auto iter = current_user->purchase_history.begin(); iter != current_user->purchase_history.end(); iter++) {
        currentUserHis.insert(*iter);
    }

    for (auto iter = user->purchase_history.begin(); iter != user->purchase_history.end(); iter++) {
        userHis.insert(*iter);
    }

    for (Product* p1 : currentUserHis) {
        for (Product* p2 : userHis) {
            if (p1->name == p2->name) {
                similarity++;
            }
        }
    }

    return similarity;
}

bool ClientUI::compare(std::pair<User*, int> p1, std::pair<User*, int> p2) {
    if (p1.second > p2.second) return true;
    else if (p1.second == p2.second) {
        if (p1.first->register_id < p2.first->register_id) {
            return true;
        }
    }
    return false;
}
