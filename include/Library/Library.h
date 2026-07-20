#ifndef LIBRARY_H
#define LIBRARY_H

#include <memory>
#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>


enum MESSAGE_IMPORTANCE {
    LOW,
    MEDIUM,
    HARD
};

enum LoggerType {
    FILE_LOGGER,
    SOCKET_LOGGER
};

class BaseLogger {
public:

    BaseLogger(MESSAGE_IMPORTANCE level) noexcept : level_{level} {}
    virtual ~BaseLogger() = default;

    void changeImportanceLevel(MESSAGE_IMPORTANCE level) noexcept { level_ = level; };
    virtual void addMessageToLog(MESSAGE_IMPORTANCE level, const char* message) = 0;

protected:

    MESSAGE_IMPORTANCE level_; ///< уровень важности сообщений по умолчанию

};

class FileLogger : public BaseLogger {
public:

    FileLogger(const char* file, MESSAGE_IMPORTANCE level) noexcept; //при инифиализации создается текстовый файл

    void addMessageToLog(MESSAGE_IMPORTANCE level, const char* message) override;

    ~FileLogger();

private:

    std::string file_; ///< имя файла журнала
    std::fstream file_flow_; ///< файловый поток, при инициализации связывается с конкретным файлом

};

class SocketLogger : public BaseLogger {
public:

    SocketLogger(const char* host_, const char* port_, MESSAGE_IMPORTANCE level) noexcept; 

    void addMessageToLog(MESSAGE_IMPORTANCE level, const char* message) override;

    void *get_in_addr(); 

    ~SocketLogger();

private:

    int socket_;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];
};

class LoggerFactory {
public:
    static std::unique_ptr<BaseLogger> CreateFileLogger(const char* file, MESSAGE_IMPORTANCE level);

    static std::unique_ptr<BaseLogger> CreateSocketLogger(const char* host, const char* port, MESSAGE_IMPORTANCE level);
};

#endif