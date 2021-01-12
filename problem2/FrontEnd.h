#ifndef PROBLEM2_FRONTEND_H
#define PROBLEM2_FRONTEND_H
#include <iostream>
#include "app.h"
#include "BackEnd.h"

class FrontEnd {
public:
    FrontEnd(BackEnd* backend, std::istream& is, std::ostream& os);
    std::string current_user;
    bool auth(const std::string&, const std::string&);
    void post();
    void recommend();
    void search(const std::string&);
private:
    BackEnd backend;
    std::istream& is;
    std::ostream& os;
};

#endif //PROBLEM2_FRONTEND_H
