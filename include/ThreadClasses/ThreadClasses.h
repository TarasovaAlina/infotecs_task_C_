#ifndef THREADCLASSES_H
#define THREADCLASSES_H

#include <mutex>
#include <thread>
#include <queue>
#include <condition_variable>

#include "Library/Library.h"

/**
 * @file Program.h
 * @brief В этом файле находится класс программы Program для взаимодействия между вводом, библиотекой и работой многопоточности.
 * @details В некотором плане реализует работы контроллера, связывая ввод пользователя с библиотекой.
 * @author Tarasova Alina
**/

/**
 * @struct log_struct
 * @brief Структура для хранения одной записи журнала в многопоточной очереди.
 * @details Используется как элемент очереди ThreadQueue и содержит всю информацию,
 * необходимую для передачи сообщения между потоками: уровень важности, текст сообщения
 * и временную метку.
 */
struct log_struct {
    MESSAGE_IMPORTANCE level; ///< Уровень важности сообщения.
    std::string message; ///< Текст сообщения.
    std::tm* time; ///< Временная метка создания сообщения.
};

/**
 * @class ThreadQueue
 * @brief Потокобезопасная очередь для обмена сообщениями между потоками.
 * @details Предоставляет безопасные методы добавления и извлечения элементов
 * из очереди. Для синхронизации доступа используется std::mutex, а для ожидания
 * появления новых элементов — std::condition_variable.
 */
class ThreadQueue {
public:

    /**
     * @brief Конструктор по умолчанию.
     * @details Инициализирует пустую очередь, мьютекс и объект синхронизации.
     */
    ThreadQueue() : queue_{}, mutex_{}, cv_{} {}

    /**
     * @brief Добавляет новый элемент в очередь.
     * @param level Уровень важности сообщения.
     * @param message Текст сообщения.
     * @param time Временная метка сообщения.
     * @details После добавления элемента уведомляет ожидающий поток
     * о появлении новых данных.
     */
    void push(MESSAGE_IMPORTANCE level, std::string& message, std::tm* time);
    
    /**
     * @brief Извлекает первый элемент из очереди.
     * @return Структуру log_struct с данными сообщения.
     * @details Если очередь пуста, поток ожидает появления новых элементов.
     * При завершении работы возвращается пустая структура.
     */
    log_struct pop();

    /**
     * @brief Завершает работу очереди.
     * @details Устанавливает флаг остановки и уведомляет все ожидающие потоки,
     * чтобы они могли корректно завершить выполнение.
     */
    void stop();

    /**
     * @brief Возвращает очередь сообщений.
     * @return Константную ссылку на внутреннюю очередь.
     * @details Используется только для чтения состояния очереди.
     */
    const std::queue<log_struct>& queue() const noexcept { return queue_; };

private:

    std::queue<log_struct> queue_; ///< Внутренняя очередь сообщений.
    mutable std::mutex mutex_; ///< Мьютекс для синхронизации доступа к очереди.
    std::condition_variable cv_; ///< Объект синхронизации для ожидания появления новых элементов.
    bool stop_flag_{0}; ///< Флаг завершения работы очереди.

};

/**
 * @brief Поток приема сообщений от пользователя.
 * @param queue Очередь для передачи введенных сообщений.
 * @param LogLibrary Указатель на объект библиотеки логирования.
 * @details Считывает строки из стандартного ввода, определяет уровень важности
 * сообщения, формирует временную метку и помещает запись в ThreadQueue.
 * При вводе команды "exit" завершает работу очереди и прекращает выполнение потока.
 */
static void acceptRequest(ThreadQueue& queue, BaseLogger* LogLibrary) {
    std::cout << "Accept thread id: " << std::this_thread::get_id() << '\n';
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
            queue.stop();
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

        } else {
            if (isdigit(str[0])) {
                int num = str[0] - '0';
                m_i = static_cast<MESSAGE_IMPORTANCE>(num);
                message = str.substr(2, str.size() - 2);
            } else {
                message = str;
                m_i = LogLibrary->level();
            }
            queue.push(m_i, message, time);
        }
    }
}

/**
 * @brief Поток отправки сообщений в библиотеку логирования.
 * @param queue Потокобезопасная очередь сообщений.
 * @param LogLibrary Указатель на объект библиотеки логирования.
 * @details Извлекает сообщения из очереди ThreadQueue и передает их
 * в библиотеку логирования посредством метода addMessageToLog().
 * Завершает выполнение после получения пустого сообщения.
 */
static void sendRequest(ThreadQueue& queue, BaseLogger* LogLibrary) {
    std::cout << "Send thread id: " << std::this_thread::get_id() << '\n';
    while (true) {
        auto value = queue.pop();

        if (value.message.empty())
            break;

        LogLibrary->addMessageToLog(value.level, value.message, value.time);
    }
}

#endif