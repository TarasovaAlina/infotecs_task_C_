#include "ThreadClasses/ThreadClasses.h"
#include <iostream>

void ThreadQueue::push(MESSAGE_IMPORTANCE level, std::string& message, std::tm* time) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push( log_struct{ level, message, time } );
    cv_.notify_one();
}

log_struct ThreadQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex_);

    cv_.wait(lock, [this]() { return !queue_.empty() || stop_flag_; } );

    if (queue_.empty() && stop_flag_) {
        return { LOW, "" , nullptr };
    }

    auto val = queue_.front();
    queue_.pop();

    return val;
}

void ThreadQueue::stop() {
    {
    std::lock_guard<std::mutex> lock(mutex_);
    stop_flag_ = true;
    }

    cv_.notify_all();
}