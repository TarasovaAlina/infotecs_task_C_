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
    std::thread acceptRequestThread(acceptRequest, std::ref(m_q_), LogLibrary.get());
    std::thread sendRequestThread(sendRequest, std::ref(m_q_), LogLibrary.get());

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
