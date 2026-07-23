#include "Library/Library.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <iostream>

SocketLogger::SocketLogger(const char* host_, const char* port_, MESSAGE_IMPORTANCE level): BaseLogger(level) {

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(std::stoi(port_)));

    if (inet_pton(AF_INET, host_, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        std::exit(1);
    }

    connect_client();
}

SocketLogger::~SocketLogger() {
    if (sock >= 0) 
        close(sock);
}

void SocketLogger::connect_client() {
    if (connect(sock, reinterpret_cast<sockaddr*>(&addr), sizeof(addr)) < 0)
    {
        perror("connect");
        std::exit(1);
    }
}

void SocketLogger::send_info(const std::string& line) {
    if (send(sock, line.c_str(), line.size(), 0) < 0) {
        perror("send");
    }
}

void SocketLogger::addMessageToLog(MESSAGE_IMPORTANCE level, std::string& message, std::tm* time) {
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
    
    std::stringstream ss;
    ss << std::put_time(time, "%Y-%m-%d %H:%M:%S");
    std::string str_time = ss.str();

    std::string res = str_time + ' ' + log_level_[level] + ' ' + message + '\n';

    send_info(res);
}