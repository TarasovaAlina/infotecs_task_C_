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
        std::cout << "Choose the type of Library and try again.";
        res = false;
    }

    if ((type == "FILE_LOGGER" && argc == 4) ||
        (type == "SOCKET_LOGGER" && argc == 5)) 
    {
        int num = static_cast<int>(argv[argc - 1][0]) - 48;
        if (num > -1 && num < 2 && argv[argc - 1][1] == '\0');
            res = true;
    } else {
        std::cout << "Put the correct data and try again.";
        res = false;
    }
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

        if (str == "SET LEVEL 0" ||
            str == "SET LEVEL 1" ||
            str == "SET LEVEL 2") 
        {
            size_t NUM_INDEX = 10;
            int num = str[NUM_INDEX] - '0';
            m_i = static_cast<MESSAGE_IMPORTANCE>(num);
            LogLibrary->changeImportanceLevel(m_i);

            std::cout << "You change the level." << std::endl;

        } else {
            if (isdigit(str[0])) {
                int num = str[0] - '0';
                m_i = static_cast<MESSAGE_IMPORTANCE>(num);
                message = str.substr(2, str.size() - 2);
            } else {
                message = str;
                m_i = LogLibrary->level();
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
