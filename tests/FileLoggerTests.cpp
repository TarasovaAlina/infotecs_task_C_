#include "Library/Library.h"
#include <gtest/gtest.h>
#include <filesystem>

// Проверка на создание объекта нужного типа
TEST(FileLogger, CreateFileLogger) {
    std::unique_ptr<BaseLogger> test_library_ = LoggerFactory::CreateFileLogger("FILE_LOGGER.txt", MESSAGE_IMPORTANCE::MEDIUM);
    EXPECT_EQ(typeid(*(test_library_.get())), typeid(FileLogger));
}

// Проверка на создание текстового файла
TEST(FileLogger, CreateFile) {
    std::unique_ptr<BaseLogger> test_library_ = LoggerFactory::CreateFileLogger("FILE_LOGGER.txt", MEDIUM);

    std::filesystem::path path = "FILE_LOGGER.txt";
    EXPECT_TRUE(std::filesystem::exists(path));
}

// Сообщения уровнем ниже заданного не записываются
TEST(FileLogger, PutLowLevelMessage) {
    std::unique_ptr<BaseLogger> test_library_ = LoggerFactory::CreateFileLogger("FILE_LOGGER.txt", MEDIUM);

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* time = std::localtime(&now_c);

    std::filesystem::path path = "FILE_LOGGER.txt";
    EXPECT_TRUE(std::filesystem::exists(path));

    std::ofstream clear("FILE_LOGGER.txt", std::ios::trunc);
    clear.close();

    std::string message_{"Message"};

    test_library_->addMessageToLog(LOW, message_, time);
    EXPECT_TRUE(std::filesystem::is_empty(path));
}

// Сообщения несуществующего уровня не записываются
TEST(FileLogger, PutIncorrectLevelMessage) {
    std::unique_ptr<BaseLogger> test_library_ = LoggerFactory::CreateFileLogger("FILE_LOGGER.txt", MEDIUM);

    std::filesystem::path path = "FILE_LOGGER.txt";

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* time = std::localtime(&now_c);

    std::string message_{"Message"};

    test_library_->addMessageToLog(static_cast<MESSAGE_IMPORTANCE>(3), message_, time);
    EXPECT_TRUE(std::filesystem::is_empty(path));
}

// Сообщение соответствующего из MESSAGE_IMPORTANCE записываются
TEST(FileLogger, PutCorrectLevelMessage) {
    std::unique_ptr<BaseLogger> test_library_ = LoggerFactory::CreateFileLogger("FILE_LOGGER.txt", MEDIUM);

    std::filesystem::path path = "FILE_LOGGER.txt";

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* time = std::localtime(&now_c);

    std::string message_{"Message"};
    
    test_library_->addMessageToLog(MEDIUM, message_, time);
    EXPECT_FALSE(std::filesystem::is_empty(path));
}

// Сообщения уровнем выше из MESSAGE_IMPORTANCE записываются; Проверка смены уровня важности
TEST(FileLogger, ChangeMessageLevel) {
    std::unique_ptr<BaseLogger> test_library_ = LoggerFactory::CreateFileLogger("FILE_LOGGER.txt", MEDIUM);

    std::filesystem::path path = "FILE_LOGGER.txt";

    std::ofstream clear("FILE_LOGGER.txt", std::ios::trunc);
    clear.close();

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* time = std::localtime(&now_c);

    std::string message_{"Message"};

    //Смена уровня по умолячанию и косвенная проверка на смену уровня
    test_library_->changeImportanceLevel(HARD);

    // Сообщения уровнем ниже заданного не записываются
    test_library_->addMessageToLog(MEDIUM, message_, time);
    EXPECT_TRUE(std::filesystem::is_empty(path));

    // Сообщения схожего уровня из MESSAGE_IMPORTANCE записываются
    test_library_->addMessageToLog(HARD, message_, time);
    EXPECT_FALSE(std::filesystem::is_empty(path));
}