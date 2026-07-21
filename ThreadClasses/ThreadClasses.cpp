#include "ThreadClasses/ThreadClasses.h"
#include <iostream>

void ThreadQueue::push(MESSAGE_IMPORTANCE level, std::string& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push( { level, message } );
    cv.notify_one();
}

std::pair<MESSAGE_IMPORTANCE, std::string> ThreadQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv.wait(lock, [this]() { return !queue_.empty(); } );
    auto val = queue_.front();
    queue_.pop();

    return val;
}

void ThreadProcessRequest::acceptRequest(ThreadQueue& queue) {
    while (true) {
        std::string str{};
        MESSAGE_IMPORTANCE m_i{};

        std::cout << "Put the importance level and the following message please...\n";
        getline(std::cin, str);

        int num = str[0] - '0';
        MESSAGE_IMPORTANCE mess_imp = static_cast<MESSAGE_IMPORTANCE>(num);
        std::string message = str.substr(2, str.size() - 2);

        queue.push(mess_imp, message);
    }
}

void ThreadProcessRequest::sendRequest(ThreadQueue& queue, BaseLogger* LogLibrary) {
    while (true) {
        auto value = queue.pop();
        LogLibrary->addMessageToLog (value.first, value.second);
    }
}