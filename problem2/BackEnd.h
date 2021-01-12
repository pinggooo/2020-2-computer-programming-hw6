#ifndef PROBLEM2_BACKEND_H
#define PROBLEM2_BACKEND_H
#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>
#include <string>
#include <filesystem>
#include <sstream>
#include <algorithm>
#include "config.h"
#include "User.h"
#include "Post.h"

class BackEnd {
public:
    BackEnd();
    bool auth(const std::string&, const std::string&);
    void post(const std::string&, const std::string&, const std::string&);
    std::vector<Post*> recommend(const std::string&);
    std::vector<Post*> search(const std::string&);
private:
    std::vector<User*> userList;
    std::vector<Post*> postList;
    User* findUser(const std::string&);
    int findMaxID();
    std::vector<Post*> getUserPost(const std::string&);
    std::vector<std::string> getUserFriend(const std::string&);
    int getKeywordNum(Post*, const std::string&);
    static bool compare(Post*, Post*);
    static bool compare_pair(std::pair<Post*, int>, std::pair<Post*, int>);
};

#endif //PROBLEM2_BACKEND_H
