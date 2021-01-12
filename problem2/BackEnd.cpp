#include "BackEnd.h"

BackEnd::BackEnd() {
    std::vector<std::string> userIDs;
    for (auto& p : std::filesystem::directory_iterator(SERVER_STORAGE_DIR)) {
        std::string userDir = p.path().string();
        int idx = userDir.find('/');
        std::string userID = userDir.substr(idx + 1);
        userIDs.push_back(userID);
    }
    for (const std::string& userID : userIDs) {
        std::ifstream pw;
        pw.open(SERVER_STORAGE_DIR + userID + "/password.txt");
        std::string password;
        std::getline(pw, password);
        User* user = new User(userID, password);
        userList.push_back(user);
        pw.close();
    }
    for (User* user : userList) {
        for (auto& p : std::filesystem::directory_iterator(SERVER_STORAGE_DIR + user->id + "/post")) {
            std::string postFile = p.path().filename().string();
            int idx = postFile.find_last_of('.');
            std::string postID;
            for (int i = 0; i < idx; i++) {
                postID += postFile[i];
            }
            std::ifstream postFile_ifs;
            std::string curTime, title, space, content;
            postFile_ifs.open(SERVER_STORAGE_DIR + user->id + "/post/" + postFile);
            std::getline(postFile_ifs, curTime);
            std::getline(postFile_ifs, title);
            std::getline(postFile_ifs, space);
            int first_input = 0;
            while (true) {
                std::string input;
                std::getline(postFile_ifs, input);
                if (input.empty() && first_input == 1) break;
                content += input + "\n";
                first_input = 1;
            }
            Post* post = new Post(std::atoi(postID.c_str()), curTime, title, content);
            post->writer = user->id;
            postList.push_back(post);
            postFile_ifs.close();
        }
    }
}

bool BackEnd::auth(const std::string& id, const std::string& password) {
    User* user = findUser(id);
    if (user == nullptr) return false;
    if (user->getPW() == password) return true;
    return false;
}

void BackEnd::post(const std::string& id, const std::string& title, const std::string& content) {
    Post* post = new Post(title, content);
    post->id = findMaxID() + 1;
    std::ofstream postFile;
    postFile.open(SERVER_STORAGE_DIR + id + "/post/" + std::to_string(post->id) + ".txt");
    postFile << post->dateTime << std::endl;
    postFile << post->title << std::endl << std::endl;
    postFile << post->content;
    postFile.close();
    post->writer = id;
    postList.push_back(post);
}

std::vector<Post*> BackEnd::recommend(const std::string& id) {
    std::vector<Post*> recommendList;
    std::vector<Post*> friendPostList;
    std::vector<std::string> friendList = getUserFriend(id);
    for (const std::string& friend_ : friendList) {
        std::vector<Post*> friendPosts = getUserPost(friend_);
        for (Post* post : friendPosts) {
            friendPostList.push_back(post);
        }
    }
    std::sort(friendPostList.begin(), friendPostList.end(), compare);
    int idx = 0;
    for (Post* post : friendPostList) {
        recommendList.push_back(post);
        idx++;
        if (idx == 10) break;
    }
    return recommendList;
}

std::vector<Post*> BackEnd::search(const std::string& keywords) {
    std::vector<std::pair<Post*, int>> posts;
    std::unordered_set<std::string> keywordSet;
    std::istringstream iss(keywords);
    int first_search = 0;
    for (std::string s; iss >> s; ) {
        if (s == "search" && first_search == 0) {
            first_search = 1;
            continue;
        }
        keywordSet.insert(s);
    }
    for (Post* post : postList) {
        int count = 0;
        for (const std::string& keyword : keywordSet) {
            count += getKeywordNum(post, keyword);
        }
        if (count != 0) {
            posts.emplace_back(post, count);
        }
    }

    std::sort(posts.begin(), posts.end(), compare_pair);
    std::vector<Post*> searched;
    int idx = 0;
    for (auto& post : posts) {
        searched.push_back(post.first);
        idx++;
        if (idx == 10) break;
    }

    return searched;
}

User* BackEnd::findUser(const std::string& id) {
    for (User* user : userList) {
        if (user->id == id) return user;
    }
    return nullptr;
}

int BackEnd::findMaxID() {
    int max = 0;
    for (Post* post : postList) {
        if (max < post->id) max = post->id;
    }
    return max;
}

std::vector<Post*> BackEnd::getUserPost(const std::string& id) {
    std::vector<Post*> posts;
    for (auto& p : std::filesystem::directory_iterator(SERVER_STORAGE_DIR + id + "/post")) {
        std::string postFile = p.path().filename().string();
        int idx = postFile.find_last_of('.');
        std::string postID;
        for (int i = 0; i < idx; i++) {
            postID += postFile[i];
        }
        std::ifstream postFile_ifs;
        std::string curTime, title, space, content;
        postFile_ifs.open(SERVER_STORAGE_DIR + id + "/post/" + postFile);
        std::getline(postFile_ifs, curTime);
        std::getline(postFile_ifs, title);
        std::getline(postFile_ifs, space);
        int first_input = 0;
        while (true) {
            std::string input;
            std::getline(postFile_ifs, input);
            if (input.empty() && first_input == 1) break;
            content += input + "\n";
            first_input = 1;
        }
        Post* post = new Post(std::atoi(postID.c_str()), curTime, title, content);
        posts.push_back(post);
        postFile_ifs.close();
    }
    return posts;
}

std::vector<std::string> BackEnd::getUserFriend(const std::string& id) {
    std::vector<std::string> friendList;
    std::ifstream friendFile;
    friendFile.open(SERVER_STORAGE_DIR + id + "/friend.txt");
    while (true) {
        std::string input;
        std::getline(friendFile, input);
        if (input.empty()) break;
        friendList.push_back(input);
    }
    friendFile.close();
    return friendList;
}

int BackEnd::getKeywordNum(Post* post, const std::string& keyword) {
    int number = 0;
    std::istringstream iss_title(post->title);
    std::vector<std::string> words_title;
    for (std::string s; iss_title>> s; ) {
        words_title.push_back(s);
    }
    std::string content = post->content;
    std::vector<std::string> words_content;
    std::istringstream iss_content(post->content);
    for (std::string s; iss_content >> s; ) {
        words_content.push_back(s);
    }
    for (const std::string& word : words_title) {
        if (word == keyword) number++;
    }
    for (const std::string& word : words_content) {
        if (word == keyword) number++;
    }
    return number;
}

bool BackEnd::compare(Post* p1, Post* p2) {
    return p1->dateTime > p2->dateTime;
}

bool BackEnd::compare_pair(std::pair<Post*, int> p1, std::pair<Post*, int> p2) {
    if (p1.second > p2.second) return true;
    else if (p1.second == p2.second) {
        if (p1.first->dateTime > p2.first->dateTime) return true;
    }
    return false;
}