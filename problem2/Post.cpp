#include "Post.h"

Post::Post(const std::string& title, const std::string& content) {
    time_t t = time(nullptr);
    struct tm *pLocal = localtime(&t);
    std::string year = std::to_string(pLocal->tm_year + 1900);
    std::string month = std::to_string(pLocal->tm_mon + 1);
    std::string day = std::to_string(pLocal->tm_mday);
    std::string hour = std::to_string(pLocal->tm_hour);
    std::string min = std::to_string(pLocal->tm_min);
    std::string sec = std::to_string(pLocal->tm_sec);
    std::string curTime = std::to_string(pLocal->tm_year + 1900) + "/" + timeFormat(month) + "/" + timeFormat(day) + " "
            + timeFormat(hour) + ":" + timeFormat(min) + ":" + timeFormat(sec);
    this->dateTime = curTime;
    this->title = title;
    this->content = content;
}

Post::Post(const int& id, const std::string& dateTime, const std::string& title, const std::string& content) {
    this->id = id;
    this->dateTime = dateTime;
    this->title = title;
    this->content = content;
}

std::string Post::timeFormat(const std::string& time) {
    if (time.length() == 1) {
        return "0" + time;
    }
    return time;
}

