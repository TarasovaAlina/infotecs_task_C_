#ifndef THREADCLASSES_H
#define THREADCLASSES_H

#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

#include "Library/Library.h"

class ThreadQueue {
public:

    ThreadQueue() : queue_{}, mutex_{}, cv{} {}

    void push(MESSAGE_IMPORTANCE level, std::string& message);
    std::pair<MESSAGE_IMPORTANCE, std::string> pop();
    void stop();

private:

    std::queue<std::pair<MESSAGE_IMPORTANCE, std::string>> queue_;
    mutable std::mutex mutex_;
    std::condition_variable cv;
    bool stop_flag_{0};

};

class ThreadProcessRequest {
public:

    static void acceptRequest(ThreadQueue& queue);
    static void sendRequest(ThreadQueue& queue, BaseLogger* LogLibrary);

};

#endif