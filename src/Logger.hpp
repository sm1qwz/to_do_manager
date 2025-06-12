#pragma once

#include <string>
#include <fstream>
#include <mutex>

/**
 * @brief Синглтон-класс для потокобезопасного логирования операций.
 */
class Logger {
public:
    /**
     * @brief Возвращает единственный экземпляр Logger.
     * @param logFilePath Путь к лог-файлу (по умолчанию \"history.log\").
     * @return Ссылка на Logger.
     */
    static Logger& instance(const std::string& logFilePath = "history.log");

    /**
     * @brief Записывает сообщение с временной меткой.
     * @param message Текст сообщения (без времени).
     */
    void log(const std::string& message);

private:
    Logger(const std::string& logFilePath);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::ofstream ofs_;  ///< Поток для записи в файл.
    std::mutex mtx_;     ///< Мьютекс для синхронизации.
};
