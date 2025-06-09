#pragma once

#include "Task.hpp"
#include <vector>
#include <optional>
#include <string>

/**
 * @brief Менеджер задач: хранит в памяти вектор Task, предоставляет операции добавления, удаления, пометки как выполненной, фильтрации и т. д.
 */
class TaskManager {
public:
    TaskManager() = default;

    /**
     * @brief Добавляет задачу с описанием и опциональной датой дедлайна и тегами.
     * @param description текст описания
     * @param dueDate дата дедлайна (опционально)
     * @param tags список тегов (опционально)
     * @return id только что созданной задачи
     */
    int addTask(const std::string& description,
                const std::optional<std::string>& dueDate = std::nullopt,
                const std::vector<std::string>& tags = {});

    /**
     * @brief Удаляет задачу по id. Если задача не найдена — выбрасывает исключение.
     * @param id уникальный идентификатор задачи
     */
    void removeTask(int id);

    /**
     * @brief Отмечает задачу как выполненную. Если id неверен — выбрасывает исключение.
     * @param id уникальный идентификатор задачи
     */
    void markDone(int id);

    /**
     * @brief Поиск задач по статусу (все, выполненные или активные).
     * @param showDone true = вернуть только выполненные, false = только активные. Если nullopt — вернуть все.
     */
    std::vector<Task> listTasks(const std::optional<bool>& showDone = std::nullopt) const;

    /**
     * @brief Поиск по подстроке в описании (регистр-нечувствительный).
     * @param substr подстрока поиска
     * @return вектор задач, в описании которых встречается substr
     */
    std::vector<Task> searchByDescription(const std::string& substr) const;

    /**
     * @brief Обновляет дату дедлайна для задачи по id.
     * @param id уникальный идентификатор задачи
     * @param newDueDate новая дата дедлайна (формат YYYY-MM-DD)
     */
    void updateDueDate(int id, const std::string& newDueDate);

    /**
     * @brief Добавляет тег задаче (если ещё нет).
     * @param id идентификатор задачи
     * @param tag тег для добавления
     */
    void addTag(int id, const std::string& tag);

    /**
     * @brief Удаляет тег у задачи (если он там есть).
     * @param id идентификатор задачи
     * @param tag тег для удаления
     */
    void removeTag(int id, const std::string& tag);

    /**
     * @brief Экспортирует все задачи в файл (JSON или CSV).
     * @param format "json" или "csv"
     * @param outPath путь к выходному файлу
     */
    void exportAll(const std::string& format, const std::string& outPath) const;

    /**
     * @brief Возвращает ссылку на внутренний вектор задач (для сереализации/Undo).
     */
    const std::vector<Task>& getAllTasks() const noexcept;
    
    /**
     * @brief Заменяет весь вектор задач (например, для Undo или после загрузки из Storage).
     */
    void setAllTasks(const std::vector<Task>& tasks);

private:
    std::vector<Task> tasks_;

    /**
     * @brief Находит индекс задачи в векторе по id. Если не найдено — возвращает -1.
     */
    int findIndexById(int id) const noexcept;

    /**
     * @brief Вспомогательная функция для валидации id (выкидывает исключение std::runtime_error, если не найден).
     */
    void ensureExists(int id) const;
};
