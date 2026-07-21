#include "Library/Library.h"
#include <iostream>

FileLogger::FileLogger(const char* file, MESSAGE_IMPORTANCE level) noexcept: BaseLogger(level), file_{file} {
    file_flow_.open(file_, std::ios::out | std::ios::app);

    std::cout << "Opening: " << file_ << '\n';
    std::cout << "is_open = " << file_flow_.is_open() << '\n';
}

FileLogger::~FileLogger() {
    if (file_flow_)
        file_flow_.close();
}

void FileLogger::addMessageToLog(MESSAGE_IMPORTANCE level, std::string& message) {
    if (level < level_)
        return;
    
    const char* new_message = message.c_str();

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    if (file_flow_.is_open()) {
        std::cout << "Write: " << message << '\n';
        file_flow_ << new_message << '\n';
        file_flow_ << "Level importance is " << level << '\n';
        file_flow_ << "Message time is " << std::put_time(std::localtime(&now_c), "%F %T") << "\n";
        file_flow_.flush();
    }
}

