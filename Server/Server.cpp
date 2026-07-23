#include "Server/Server.h"

#include <arpa/inet.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>

Server::Server(const char* host_, const char* port_, const char* N)
    : listener{-1}, accept_sock{-1}, N_{static_cast<size_t>(std::stoul(N))}
{
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener < 0) {
        perror("socket");
        std::exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(static_cast<uint16_t>(std::stoi(port_)));

    if (inet_pton(AF_INET, host_, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(listener);
    }
}

Server::~Server() {
    if (accept_sock >= 0)
        close(accept_sock);

    if (listener >= 0)
        close(listener);
}

void Server::bind_server() {
    if (bind(listener,
             reinterpret_cast<sockaddr*>(&addr),
             sizeof(addr)) < 0)
    {
        perror("bind");
        std::exit(1);
    }
}

void Server::listen_server(int backlog) {
    if (listen(listener, backlog) < 0) {
        perror("listen");
        std::exit(1);
    }
}

int Server::accept_client() {
    accept_sock = accept(listener, nullptr, nullptr);

    if (accept_sock < 0) {
        perror("accept");
    }

    return accept_sock;
}

void Server::send_info(const std::string& line) {
    if (send(accept_sock, line.c_str(), line.size(), 0) < 0) {
        perror("send");
    }
}

std::string Server::read_info() {
    return read_line(accept_sock);
}

void Server::close_client_socket() {
    if (accept_sock >= 0) {
        close(accept_sock);
        accept_sock = -1;
    }
}

std::string Server::read_line(int sock) {
    std::string str;
    char ch;

    while (recv(sock, &ch, 1, 0) > 0) {
        if (ch == '\n')
            break;

        str += ch;
    }

    return str;
}

void Server::processStatistics(const std::string& line) {
    std::istringstream iss(line);

    std::string date;
    std::string time;
    std::string level;
    std::string message;

    iss >> date >> time >> level;
    std::getline(iss, message);

    if (!message.empty() && message[0] == ' ')
        message.erase(0, 1);

    statistics_.all_message_count_++;

    if (level == "LOW") statistics_.low_message_count_++;
    if (level == "MEDIUM") statistics_.medium_message_count_++;
    if (level == "HARD") statistics_.hard_message_count_++;

    statistics_.min_len_ = (statistics_.min_len_ > message.size()) ? message.size() : statistics_.min_len_;
    statistics_.max_len_ = (statistics_.max_len_ < message.size()) ? message.size() : statistics_.max_len_;
    statistics_.average_len_sum_ +=  message.size();
    statistics_.average_len_ = statistics_.average_len_sum_ / statistics_.all_message_count_;

    if (statistics_.all_message_count_ == N_) {
        PutInformation();
    }
}

void Server::PutInformation() {
    std::cout << "Total messages: " << statistics_.all_message_count_ << '\n';

    std::cout << "Messages by level:\n";
    std::cout << "  LOW: " << statistics_.low_message_count_ << '\n';
    std::cout << "  MEDIUM: " << statistics_.medium_message_count_ << '\n';
    std::cout << "  HARD: " << statistics_.hard_message_count_ << '\n';

    std::cout << "\nMessage length statistics:\n";
    std::cout << "  Minimum length: " << statistics_.min_len_ << '\n';
    std::cout << "  Maximum length: " << statistics_.max_len_ << '\n';
    std::cout << "  Average length: " << statistics_.average_len_ << '\n';
}