#pragma once

#include <string>
#include <fstream>
#include <mutex>

/**
 * @brief Логгер для записи истории операций. Потокобезопасен.
 */
class Logger {
public:
    /**
     * @brief Единственный экземпляр (синглтон).
     * @param logFilePath путь к лог-файлу
     * @return ссылка на Logger
     */
    static Logger& instance(const std::string& logFilePath = "history.log");

    /**
     * @brief Записывает сообщение в лог с меткой времени.
     * @param message текст сообщения (без времени)
     */
    void log(const std::string& message);

private:
    Logger(const std::string& logFilePath);
    ~Logger();
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream ofs_;
    std::mutex mtx_;
};
