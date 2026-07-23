#ifndef LIBRARY_H
#define LIBRARY_H

#include <map>
#include <memory>
#include <string>
#include <fstream>
#include <chrono>

#include <sys/socket.h>
#include <netinet/in.h>

/**
 * @file Library.h
 * @brief В этом файле находятся абстракный класс BaseLogger и классы библиотек для работы с файлом .txt и с сокетами.
 * @author Tarasova Alina
 * 
 * @section design_patterns
 * 
 * 1. @b Стратегия (Strategy)
 *    - Классы: BaseLogger, FileLogger, SocketLogger
 * 
 * 2. @b Фабрика (Factory)
 *    - Класс: LoggerFactory
 * 
 * 3. @b Шаблонный метод (Template Method)
 *    - Классы: FileLogger, SocketLogger
 */

/**
 * @enum MESSAGE_IMPORTANCE
 * @brief Уровни важности сообщений
 */
enum MESSAGE_IMPORTANCE {
    LOW, ///< Низкий уровень важности
    MEDIUM, ///< Средний уровень важности
    HARD ///< Высокий уровень важности
};

/**
 * @enum LoggerType
 * @brief Тип библиотеки
 */
enum LoggerType {
    FILE_LOGGER, ///< Тип библиотеки для работы с текстовым файлом
    SOCKET_LOGGER ///< Тип библиотеки для работы с сокетом
};

/**
 * @class BaseLogger
 * @brief Абстракный класс библиотеки с общими методами приема сообщения и установки уровня
 */
class BaseLogger {
public:

    /**
     * @brief Конструктор с параметрами
     * @param level Уровень по умолчанию
     */
    BaseLogger(MESSAGE_IMPORTANCE level) noexcept : 
        level_{level}, 
        log_level_{
            {MESSAGE_IMPORTANCE::LOW, "LOW"},
            {MESSAGE_IMPORTANCE::MEDIUM, "MEDIUM"},
            {MESSAGE_IMPORTANCE::HARD, "HARD"}
        }
        {}
    virtual ~BaseLogger() = default;

    /**
     * @brief Метод смены уровня по умолччанию.
     * @param level Новый уровень из MESSAGE_IMPORTANCE.
     */
    void changeImportanceLevel(MESSAGE_IMPORTANCE level) noexcept { level_ = level; };

    /**
     * @brief Метод добавления лога в текстовый файл.
     * @param level Уровень сообщения. В методе проверяется корретсность переданого уровня.
     * @param message Переданное сообщение.
     * @param time Временная метка. Заносится именно время передачи лога, а не записи, что важно
     * в многопоточном режиме.
     * @details В случае если пользователь не передал уровень, устанавливается LOW уровень и 
     * сообщение заносится в журнал
     */
    virtual void addMessageToLog(MESSAGE_IMPORTANCE level, std::string& message, std::tm* time) = 0;

    /**
     * @brief Геттер для получения уровня по умолчанию.
     */
    MESSAGE_IMPORTANCE level() const noexcept { return level_; };

protected:

    std::map<MESSAGE_IMPORTANCE, std::string> log_level_; ///< map для записи уровня в текстовый журнал
    MESSAGE_IMPORTANCE level_; ///< уровень важности сообщений по умолчанию

};

/**
 * @class FileLogger
 * @brief Класс библиотеки для работы с текстовым файлом
 * @details Создает текстовый файл и задает уровень по умолчанию при инициализации.
 * Уровень возможно сменить во время работы.
 */
class FileLogger : public BaseLogger {
public:

    /**
     * @brief Конструктор с параметрами
     * @param file Имя файла
     * @param level Уровень по умолчанию
     * @details При инициализации создается текстовый файл
     */
    FileLogger(const char* file, MESSAGE_IMPORTANCE level) noexcept; 

    void addMessageToLog(MESSAGE_IMPORTANCE level, std::string& message, std::tm* time) override;

    ~FileLogger();

private:

    std::string file_; ///< имя файла журнала
    std::fstream file_flow_; ///< файловый поток, при инициализации связывается с конкретным файлом

};

/**
 * @class SocketLogger
 * @brief Класс библиотеки логирования через сокет.
 * @details Создает TCP-соединение с сервером логирования и отправляет
 * сообщения через установленное соединение. Поддерживает изменение
 * уровня важности сообщений во время работы.
 */
class SocketLogger : public BaseLogger {
public:

    /**
     * @brief Конструктор с параметрами.
     * @param host_ IP-адрес сервера для подключения.
     * @param port_ Порт сервера.
     * @param level Уровень важности сообщений по умолчанию.
     * @details При создании объекта устанавливает параметры подключения
     * и выполняет подключение к серверу.
     */
    SocketLogger(const char* host_, const char* port_, MESSAGE_IMPORTANCE level);

    ~SocketLogger();

    /**
     * @brief Добавляет сообщение в журнал.
     * @param level Уровень важности сообщения.
     * @param message Текст сообщения.
     * @param time Временная метка создания сообщения.
     * @details Проверяет уровень важности сообщения относительно установленного
     * уровня фильтрации, формирует строку лога с датой, временем и уровнем
     * важности, после чего отправляет ее через сокет.
     */
    void addMessageToLog(MESSAGE_IMPORTANCE level,
                         std::string& message,
                         std::tm* time) override;


private:

    /**
     * @brief Устанавливает соединение с сервером.
     * @details Выполняет подключение созданного сокета к указанному адресу.
     */
    void connect_client();


    /**
     * @brief Отправляет строку через сокет.
     * @param line Сообщение для отправки.
     */
    void send_info(const std::string& line);

    int sock; ///< Дескриптор клиентского сокета.

    struct sockaddr_in addr; ///< Структура с адресом и параметрами подключения.

};

/**
 * @class LoggerFactory
 * @brief Класс для создания нужной библиотеки
 * @details Возвращает указатель на созданую библиотеку, используется в классе Program
 * для инициализации библиотеки выбранного пользователем типа.
 */
class LoggerFactory {
public:
    /**
     * @brief Метод для возврата указателя unique_ptr на объект FileLogger
     * @param file Имя файла
     * @param level Уровень по умолчанию
     */
    static std::unique_ptr<BaseLogger> CreateFileLogger(const char* file, MESSAGE_IMPORTANCE level) {
        return std::make_unique<FileLogger>(file, level);
    };

    /**
     * @brief Метод для возврата указателя unique_ptr на объект SocketLogger
     * @param host_ Номер хоста
     * @param port_ Номер порта
     * @param level Уровень по умолчанию
     */
    static std::unique_ptr<BaseLogger> CreateSocketLogger(const char* host, const char* port, MESSAGE_IMPORTANCE level) {
        return std::make_unique<SocketLogger>(host, port, level);
    }
};

#endif