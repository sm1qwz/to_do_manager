#pragma once

#include "Task.hpp"
#include <string>
#include <vector>

/**
 * @brief Класс для загрузки и сохранения списка задач (JSON или SQLite).
 */
class Storage {
public:
    /**
     * @brief Конструктор.
     * @param dataFilePath Путь к файлу хранения (например, tasks.json или tasks.db).
     * @param format       \"json\" или \"sqlite\".
     */
    Storage(const std::string& dataFilePath, const std::string& format);

    /**
     * @brief Загружает все задачи из файла.
     * @return Вектор считанных задач (пустой, если файл отсутствует).
     * @throws std::runtime_error При ошибках доступа к файлу или БД.
     */
    std::vector<Task> load();

    /**
     * @brief Сохраняет список задач в файл (перезаписывает).
     * @param tasks Вектор задач для сохранения.
     * @throws std::runtime_error При ошибках записи.
     */
    void save(const std::vector<Task>& tasks);

private:
    std::string dataFilePath_; ///< Путь к файлу хранения.
    std::string format_;       ///< Формат хранения: \"json\" или \"sqlite\".
};
