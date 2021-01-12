#include <iostream>
#include "app.h"
#include "FrontEnd.h"

App::App(std::istream& is, std::ostream& os): is(is), os(os) {
    // TODO
}

void App::run() {
    // TODO
    BackEnd* backend = new BackEnd();
    FrontEnd* frontend = new FrontEnd(backend, is, os);
    std::string id, password;
    std::string command;
    os << "------ Authentication ------" << std::endl;
    os << "id=";
    std::getline(is, id);
    os << "passwd=";
    std::getline(is, password);
    if (frontend->auth(id, password)) {
        do {
            os << "-----------------------------------" << std::endl;
            os << id << "@sns.com" << std::endl;
            os << "post : Post contents" << std::endl;
            os << "recommend : recommend interesting posts" << std::endl;
            os << "search <keyword> : List post entries whose contents contain <keyword>" << std::endl;
            os << "exit : Terminate this program" << std::endl;
            os << "-----------------------------------" << std::endl;
            os << "Command=";
            std::getline(is, command);

            if (command == "exit") {
                break;
            } else if (command == "post") {
                frontend->post();
            } else if (command == "recommend") {
                frontend->recommend();
            } else if (command.substr(0, 7) == "search ") {
                frontend->search(command);
            } else if (command.substr(0, 6) == "search" && command.length() == 6) {
                command = command + " ";
                frontend->search(command);
            }
        } while (true);
    } else {
        os << "Failed Authentication." << std::endl;
    }

    delete frontend;
    delete backend;
}
