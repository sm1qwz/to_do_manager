#pragma once

#include "Task.hpp"
#include <string>
#include <vector>

/**
 * @brief Класс Storage отвечает за загрузку и сохранение списка задач в файл (JSON или SQLite).
 */
class Storage {
public:
    /**
     * @brief Конструктор. 
     * @param dataFilePath путь к файлу (например, tasks.json или tasks.db)
     * @param format "json" или "sqlite"
     */
    Storage(const std::string& dataFilePath, const std::string& format);

    /**
     * @brief Загружает все задачи из файла.
     * @return вектор объектов Task
     */
    std::vector<Task> load();

    /**
     * @brief Сохраняет все задачи в файл (перезаписывает).
     * @param tasks вектор задач для сохранения
     */
    void save(const std::vector<Task>& tasks);

private:
    std::string dataFilePath_;
    std::string format_;
};
