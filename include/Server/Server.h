#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdint>
#include <limits>

/**
 * @struct Statistics
 * @brief Структура для хранения статистики принятых сообщений.
 * @details Содержит количество сообщений по уровням важности,
 * а также статистику по длине сообщений.
 */
struct Statistics {

    size_t all_message_count_{}; ///< Общее количество принятых сообщений.
    size_t low_message_count_{}; ///< Количество сообщений с уровнем LOW.
    size_t medium_message_count_{}; ///< Количество сообщений с уровнем MEDIUM.
    size_t hard_message_count_{}; ///< Количество сообщений с уровнем HARD.

    size_t min_len_{SIZE_MAX}; ///< Минимальная длина сообщения.
    size_t max_len_{}; ///< Максимальная длина сообщения.
    size_t average_len_sum_{}; ///< Суммарная длина всех сообщений для вычисления среднего.
    size_t average_len_{}; ///< Средняя длина сообщения.
};

/**
 * @class Server
 * @brief Класс серверного приложения для приема логов через сокет.
 * @details Отвечает за создание TCP-соединения, прием сообщений от клиента
 * и сбор статистики по полученным логам.
 */
class Server {
public:

    /**
     * @brief Конструктор сервера.
     * @param host_ IP-адрес сервера.
     * @param port_ Порт для прослушивания соединения.
     * @param N Количество сообщений, после которого необходимо вывести статистику.
     */
    Server(const char* host_, const char* port_, const char* N);

    /**
     * @brief Деструктор сервера.
     * @details Закрывает открытые сокеты при завершении работы.
     */
    ~Server();


    /**
     * @brief Привязывает серверный сокет к указанному адресу и порту.
     */
    void bind_server();


    /**
     * @brief Запускает прослушивание входящих подключений.
     * @param n Максимальное количество ожидающих соединений в очереди.
     */
    void listen_server(int n);


    /**
     * @brief Принимает подключение клиента.
     * @return Дескриптор сокета клиента.
     */
    int accept_client();


    /**
     * @brief Отправляет данные клиенту через сокет.
     * @param line Строка для отправки.
     */
    void send_info(const std::string& line);


    /**
     * @brief Получает строку данных от клиента.
     * @return Принятое сообщение.
     */
    std::string read_info();


    /**
     * @brief Закрывает сокет текущего клиента.
     */
    void close_client_socket();


    /**
     * @brief Обрабатывает полученное сообщение и обновляет статистику.
     * @param line Строка с данными лога.
     * @details Извлекает дату, время, уровень важности и текст сообщения,
     * после чего обновляет счетчики статистики.
     */
    void processStatistics(const std::string& line);


    /**
     * @brief Выводит текущую статистику работы сервера в консоль.
     */
    void PutInformation();


private:

    /**
     * @brief Читает строку из указанного сокета.
     * @param sock Дескриптор сокета.
     * @return Полученная строка.
     */
    std::string read_line(int sock);


    int listener{}; ///< Сокет сервера для прослушивания подключений.
    int accept_sock{}; ///< Сокет подключенного клиента.

    size_t N_; ///< Количество сообщений для автоматического вывода статистики.

    struct sockaddr_in addr; ///< Адрес сервера и параметры подключения.

    Statistics statistics_; ///< Текущая статистика принятых сообщений.

};

#endif