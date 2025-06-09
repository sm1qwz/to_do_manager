cmake_minimum_required(VERSION 3.15)
project(ToDoManager VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1) Добавляем опцию выбора формата (json или sqlite)
#    Но сам формат передаётся из --store-format при запуске исполняемого.
#    CMake здесь нужен для сборки, а не для выбора run-time-опций.

# 2) Включаем директорию с include (если есть)
include_directories(${CMAKE_SOURCE_DIR}/src)

# 3) Добавляем FetchContent для nlohmann/json
include(FetchContent)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0 # или любая актуальная версия
)
FetchContent_MakeAvailable(nlohmann_json)

# 4) Если используем SQLite, находим библиотеку
find_package(SQLite3 REQUIRED)

# 5) Собираем исполняемый файл
add_executable(ToDoManager
    src/main.cpp
    src/Task.cpp
    src/TaskManager.cpp
    src/Storage.cpp
    src/CLIParser.cpp
    src/Logger.cpp
    src/UndoStack.cpp
)

# 6) Линкуем зависимости
target_link_libraries(ToDoManager
    PRIVATE
        nlohmann_json::nlohmann_json
        SQLite::SQLite3
)

# 7) Указываем кодировку UTF-8
add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)

# 8) Устанавливаем clang-format (рекомендуется запускать отдельно)
#    Можно добавить цель:
find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT_EXE}
                -i
                -style=file
                ${PROJECT_SOURCE_DIR}/src/*.cpp
                ${PROJECT_SOURCE_DIR}/src/*.hpp
                ${PROJECT_SOURCE_DIR}/tests/*.cpp
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Running clang-format on source files"
    )
endif()

# 9) Поддиректория с тестами
enable_testing()
add_subdirectory(tests)
