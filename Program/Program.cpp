#include "Program/Program.h"

Program::Program(int argc, char **argv) noexcept: m_q_{} {
    std::string type = argv[1];

    if (type == "FILE_LOGGER") {
        int num = argv[3][0] - '0';
        MESSAGE_IMPORTANCE level = static_cast<MESSAGE_IMPORTANCE>(num);
        LogLibrary = LoggerFactory::CreateFileLogger(argv[2], level);
        std::cout << "CreateFileLogger\n";
    }
    else if (type == "SOCKET_LOGGER") {
        int num = argv[4][0] - '0';
        MESSAGE_IMPORTANCE level = static_cast<MESSAGE_IMPORTANCE>(num);
        LogLibrary = LoggerFactory::CreateSocketLogger(argv[2], argv[3], level);
        std::cout << "CreateSocketLogger\n";
    }
}

void Program::processRequests() noexcept {
    std::thread acceptRequestThread(ThreadProcessRequest::acceptRequest, std::ref(m_q_));
    std::thread sendRequestThread(ThreadProcessRequest::sendRequest, std::ref(m_q_), LogLibrary.get());

    acceptRequestThread.join();
    sendRequestThread.join();
}
