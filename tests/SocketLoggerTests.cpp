#include "Library/Library.h"
#include <gtest/gtest.h>

// Проверка на создание объекта нужного типа
TEST(SocketLogger, test_2) {
    std::unique_ptr<BaseLogger> test_library_ = LoggerFactory::CreateSocketLogger("127.0.0.7", "3479", MEDIUM);
    EXPECT_EQ(typeid(*(test_library_.get())), typeid(SocketLogger));

}