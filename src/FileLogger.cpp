#include "Library/Library.h"
#include <iostream>
#include <iomanip>

FileLogger::FileLogger(const char* file, MESSAGE_IMPORTANCE level) noexcept: BaseLogger(level), file_{file} {
    file_flow_.open(file_, std::ios::out | std::ios::app);
    file_flow_.close();
}

FileLogger::~FileLogger() {
    if (file_flow_)
        file_flow_.close();
}

void FileLogger::addMessageToLog(MESSAGE_IMPORTANCE level, std::string& message, std::tm* time) {
    if (level < level_)
        return;
    if (level > MESSAGE_IMPORTANCE::HARD) {
        std::cout << "Incorrect level." << std::endl;
        return;
    }
    if (!message.size()) {
        std::cout << "Put the message." << std::endl;
        return;
    }
    
    file_flow_.open(file_, std::ios::out | std::ios::app);

    if (!file_flow_.is_open()) {
        std::cout << "File is not open." << std::endl;
        return;
    }

    const char* new_message = message.c_str();

    file_flow_ << std::put_time(time, "%F %T") << ' ';
    file_flow_ << log_level_[level] << ' ';
    file_flow_ << new_message << std::endl;
    file_flow_.close();
}

