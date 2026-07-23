#include "Library/Library.h"
#include "ThreadClasses/ThreadClasses.h"
#include <gtest/gtest.h>

// Проверка на работу push и pop
TEST(ThreadClasses, PopPushMethod) {

    ThreadQueue threadqueue_;
    std::string message_{"Message"};

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* time = std::localtime(&now_c);

    const log_struct log_struct_{ LOW, message_, time };

    threadqueue_.push(LOW, message_, time);
    auto queue_ = threadqueue_.queue();
    auto threadqueue_el_ = threadqueue_.pop();

    EXPECT_EQ((queue_.front()).level, log_struct_.level);
    EXPECT_EQ((queue_.front()).message, log_struct_.message);

    EXPECT_EQ(threadqueue_el_.level, log_struct_.level);
    EXPECT_EQ(threadqueue_el_.message, log_struct_.message);
}

// Проверка на ожидание очереди пока не появится элемент
TEST(ThreadClasses, EmptyQueue) {

    ThreadQueue threadqueue_;
    std::string message_{"Message"};
    log_struct res{};

    std::thread cons([&]()
    {
        auto value_ = threadqueue_.pop();
        res = value_;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* time = std::localtime(&now_c);

    const log_struct log_struct_{LOW, message_, time };

    threadqueue_.push(LOW, message_, time);
    cons.join();

    EXPECT_EQ(res.level, log_struct_.level);
    EXPECT_EQ(res.message, log_struct_.message);
}

// Проверка на остановку работы программы
TEST(ThreadClasses, StopMethod) {

    ThreadQueue threadqueue_;
    std::string message_{"Message"};
    bool end_flag_{};

    std::thread cons([&]()
    {
        threadqueue_.pop();
        end_flag_ = true;
    });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* time = std::localtime(&now_c);

    const log_struct log_struct_{LOW, message_, time };

    threadqueue_.stop();
    cons.join();

    EXPECT_TRUE(end_flag_);
}