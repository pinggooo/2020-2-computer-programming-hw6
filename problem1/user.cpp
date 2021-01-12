#include "user.h"

User::User(std::string name, std::string password): name(name), password(password), premium(false), cart(), purchase_history() {}

std::string User::getPW() {
    return password;
}
