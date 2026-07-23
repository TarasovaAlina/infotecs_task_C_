#include "Program/Program.h"
#include <iostream>

Program::Program(int argc, char **argv) noexcept: m_q_{} {
    std::string type = argv[1];
    int num = static_cast<int>(argv[argc - 1][0]) - 48;

    if (type == "FILE_LOGGER") {
        MESSAGE_IMPORTANCE level = static_cast<MESSAGE_IMPORTANCE>(num);
        LogLibrary = LoggerFactory::CreateFileLogger(argv[2], level);
        std::cout << "CreateFileLogger\n";
    }
    else if (type == "SOCKET_LOGGER") {
        MESSAGE_IMPORTANCE level = static_cast<MESSAGE_IMPORTANCE>(num);
        LogLibrary = LoggerFactory::CreateSocketLogger(argv[2], argv[3], level);
        std::cout << "CreateSocketLogger\n";
    }
}

void Program::processRequests() noexcept {
    std::thread acceptRequestThread(&Program::acceptRequest, this);
    std::thread sendRequestThread(&Program::sendRequest, this);

    acceptRequestThread.join();
    sendRequestThread.join();
}

bool checkInputValues(int argc, char** argv) noexcept {
    bool res{};

    if (argc < 0) {
        std::cout << "Choose the type of Library and try again.";
        res = false;
    }
    
    std::string type = argv[1];
    if (type != "FILE_LOGGER" && type != "SOCKET_LOGGER")
    {
        std::cout << "Choose the type of Library and try again." << std::endl;
        res = false;
    }

    bool isCorrectType =
        (type == "FILE_LOGGER" && argc == 4) ||
        (type == "SOCKET_LOGGER" && argc == 5);

    if (!isCorrectType) {
        std::cout << "Put the correct data and try again.";
        return false;
    }

    char level = argv[argc - 1][0];

    if (argv[argc - 1][1] != '\0' || level < '0' || level > '2') {
        std::cout << "Put the correct data and try again.";
        return false;
    }

    return true;
    return res;
}

void Program::acceptRequest() {
    while (true) {
        
        std::string str{}, message{};
        MESSAGE_IMPORTANCE m_i{};

        std::cout << R"(Put the importance level and the following message please...
            Paste "SET LEVEL <NEW LEVEL>" to set new level.
            You can set 0, 1, 2 level.
            )";

        getline(std::cin, str);

        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::tm* time = std::localtime(&now_c);

        if (str == "exit") {
            m_q_.stop();
            break;
        }

        const std::string command = "SET LEVEL ";

        if (str.compare(0, command.size(), command) == 0 &&
            str.size() == command.size() + 1 &&
            str.back() >= '0' &&
            str.back() <= '2')
        {
            m_i = static_cast<MESSAGE_IMPORTANCE>(str.back() - '0');
            LogLibrary->changeImportanceLevel(m_i);

            std::cout << "You changed the level.\n";
        }
        else
        {
            if (!str.empty() && std::isdigit(static_cast<unsigned char>(str[0]))) {
                m_i = static_cast<MESSAGE_IMPORTANCE>(str[0] - '0');
                message = str.substr(2);
            } else {
                m_i = LogLibrary->level();
                message = str;
            }

            m_q_.push(m_i, message, time);
        }
    }
}


void Program::sendRequest() {
    while (true) {
        auto value = m_q_.pop();

        if (value.message.empty())
            break;

        LogLibrary->addMessageToLog(value.level, value.message, value.time);
    }
}
