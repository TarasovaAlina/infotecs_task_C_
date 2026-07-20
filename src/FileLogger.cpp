#include "Library/Library.h"

FileLogger::FileLogger(const char* file, MESSAGE_IMPORTANCE level) noexcept: BaseLogger(level), file_{file}, file_flow_{file, std::ios::app} {}

FileLogger::~FileLogger() {
    if (file_flow_)
        file_flow_.close();
}

void FileLogger::addMessageToLog(MESSAGE_IMPORTANCE level, const char* message) {
    if (level < level_)
        return;

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);

    if (file_flow_.is_open()) {
        file_flow_ << message << '\n';
        file_flow_ << "Level importance is " << level << '\n';
        file_flow_ << "Message time is " << std::put_time(std::localtime(&now_c), "%F %T") << "\n";
    }
}

