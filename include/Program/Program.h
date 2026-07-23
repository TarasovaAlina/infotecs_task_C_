#ifndef PROGRAM_H
#define PROGRAM_H

#include "Library/Library.h"
#include "ThreadClasses/ThreadClasses.h"

/**
 * @file Program.h
 * @brief В этом файле находится класс программы Program для взаимодействия между вводом, библиотекой и работой многопоточности.
 * @details В некотором плане реализует работы контроллера, связывая ввод пользователя с библиотекой.
 * @author Tarasova Alina
**/

/**
 * @class Program
 * @brief Класс приложения работы программы
 */
class Program {
public:

    /**
     * @brief Конструктор с параметрами
     * @param argc Количество введеных параметров
     * @param argv Массив введеных элементов командной строки
     */
    Program(int argc, char **argv) noexcept;

    /**
     * @brief Метод обработки логов и их передачи в текстовый файл/сокет.
     * @details Основной метод работы приложения.
     */
    void processRequests() noexcept;

private:

    std::unique_ptr<BaseLogger> LogLibrary; ///< Объект журнала. Тип выбирается в конструкторе класса.
    ThreadQueue m_q_; ///< Объект многопоточной очереди

};

/**
 * @brief Функция для обработки параметров командной строки.
 * Выводит сообщение в консоль. Предупреждает пользователя
 * в случае некорректного ввода или передает данные дальше,
 * если они были верно введены.
 * @param argc Количество введеных параметров
 * @param argv Массив введеных элементов командной строки
 */
bool checkInputValues(int argc, char** argv) noexcept;

#endif