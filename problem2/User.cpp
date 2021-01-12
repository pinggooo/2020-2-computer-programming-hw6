#include "User.h"

User::User(const std::string& id, const std::string& password) {
    this->id = id;
    this->password = password;
}

std::string User::getPW() {
    return password;
}
