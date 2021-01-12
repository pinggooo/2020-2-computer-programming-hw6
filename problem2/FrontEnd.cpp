#include "FrontEnd.h"

FrontEnd::FrontEnd(BackEnd* backend, std::istream& is, std::ostream& os) : backend(*backend), is(is), os(os), current_user() {}

bool FrontEnd::auth(const std::string& id, const std::string& password) {
    if (backend.auth(id, password)) {
        current_user = id;
        return true;
    }
    return false;
}

void FrontEnd::post() {
    std::string title, content;
    os << "-----------------------------------" << std::endl;
    os << "New Post" << std::endl;
    os << "* Title=";
    std::getline(is, title);
    os << "* Content" << std::endl;
    int first_input = 0;
    while (true) {
        std::string input;
        os << ">";
        std::getline(is, input);
        if (input.empty() && first_input == 1) {
            break;
        }
        content += input + "\n";
        first_input = 1;
    }
    backend.post(current_user, title, content);
}

void FrontEnd::recommend() {
    std::vector<Post*> friend_posts = backend.recommend(current_user);
    for (auto iter = friend_posts.begin(); iter != friend_posts.end(); iter++) {
        os << "-----------------------------------" << std::endl;
        os << "id: " << (*iter)->id << std::endl;
        os << "created at: " << (*iter)->dateTime << std::endl;
        os << "title: " << (*iter)->title << std::endl;
        os << "content:" << std::endl;
        os << (*iter)->content;
    }
}

void FrontEnd::search(const std::string& keywords) {
    std::vector<Post*> searched = backend.search(keywords);
    os << "-----------------------------------" << std::endl;
    for (auto iter = searched.begin(); iter != searched.end(); iter++) {
        os << "id: " << (*iter)->id << ", ";
        os << "created at: " << (*iter)->dateTime << ", ";
        os << "title: " << (*iter)->title << std::endl;
    }
}


