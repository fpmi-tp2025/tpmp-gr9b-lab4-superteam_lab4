cmake_minimum_required(VERSION 3.10)
project(TourismBureau)

set(CMAKE_C_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Настройка SQLite3
find_package(SQLite3 REQUIRED)

# Главное приложение (C)
add_executable(tourism_bureau
    src/main.c
    src/auth.c
    src/db_ops.c
    src/reports.c
)

target_include_directories(tourism_bureau PRIVATE includes)
target_link_libraries(tourism_bureau PRIVATE SQLite::SQLite3)

# Тесты (C++)
enable_language(CXX)  # Включаем поддержку C++ только для тестов
find_package(GTest REQUIRED)

add_executable(tourism_tests
    tests/test_auth.cpp  # Изменяем расширение на .cpp
    src/auth.c
)

target_include_directories(tourism_tests PRIVATE includes)
target_link_libraries(tourism_tests 
    PRIVATE 
    GTest::GTest 
    GTest::Main 
    SQLite::SQLite3
)
