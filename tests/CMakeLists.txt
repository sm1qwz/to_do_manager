cmake_minimum_required(VERSION 3.15)

# Подключаем GoogleTest через FetchContent или предположим, что он установлен
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.11.0
)
FetchContent_MakeAvailable(googletest)

# Создаём библиотеку с исходниками (использовать те же .cpp – для инклуда)
add_library(ToDoCore
    ../src/Task.cpp
    ../src/TaskManager.cpp
    ../src/Storage.cpp
    ../src/CLIParser.cpp
    ../src/Logger.cpp
    ../src/UndoStack.cpp
)
target_link_libraries(ToDoCore
    PRIVATE
        nlohmann_json::nlohmann_json
        SQLite::SQLite3
)

# Добавляем сами тесты
add_executable(ToDoTests
    TestTask.cpp
    TestTaskManager.cpp
    TestStorage.cpp
    TestCLIParser.cpp
)

target_link_libraries(ToDoTests
    PRIVATE
        ToDoCore
        gtest_main
)

# Добавляем тесты в ctest
include(GoogleTest)
gtest_discover_tests(ToDoTests)
