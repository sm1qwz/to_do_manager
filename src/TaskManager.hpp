#pragma once

#include "Task.hpp"
#include <vector>
#include <optional>
#include <string>

/**
 * @brief Менеджер задач: хранит и управляет списком Task.
 */
class TaskManager {
public:
    /**
     * @brief Добавляет новую задачу.
     * @param description Описание задачи.
     * @param dueDate     Дата дедлайна (опционально).
     * @param tags        Список тегов (опционально).
     * @return Сгенерированный уникальный идентификатор задачи.
     */
    int addTask(const std::string& description,
                const std::optional<std::string>& dueDate = std::nullopt,
                const std::vector<std::string>& tags = {});

    /**
     * @brief Удаляет задачу по ID.
     * @param id Идентификатор задачи.
     * @throws std::runtime_error Если задача с таким ID не найдена.
     */
    void removeTask(int id);

    /**
     * @brief Помечает задачу как выполненную.
     * @param id ID задачи.
     * @throws std::runtime_error Если задача не найдена.
     */
    void markDone(int id);

    /**
     * @brief Возвращает список задач по фильтру.
     * @param showDone Если true — только выполненные; false — только активные; nullopt — все.
     * @return Вектор задач, соответствующих фильтру.
     */
    std::vector<Task> listTasks(const std::optional<bool>& showDone = std::nullopt) const;

    /**
     * @brief Ищет задачи по подстроке в описании (регистр-независимо).
     * @param substr Подстрока для поиска.
     * @return Вектор задач, в описании которых встречается substr.
     */
    std::vector<Task> searchByDescription(const std::string& substr) const;

    /**
     * @brief Обновляет дедлайн существующей задачи.
     * @param id        Идентификатор задачи.
     * @param newDueDate Новая дата дедлайна (YYYY-MM-DD).
     * @throws std::runtime_error Если задача не найдена.
     */
    void updateDueDate(int id, const std::string& newDueDate);

    /**
     * @brief Добавляет тег к задаче.
     * @param id  Идентификатор задачи.
     * @param tag Метка для добавления.
     * @throws std::runtime_error Если задача не найдена.
     */
    void addTag(int id, const std::string& tag);

    /**
     * @brief Удаляет тег у задачи.
     * @param id  Идентификатор задачи.
     * @param tag Метка для удаления.
     * @throws std::runtime_error Если задача не найдена.
     */
    void removeTag(int id, const std::string& tag);

    /**
     * @brief Экспортирует все задачи в файл.
     * @param format  \"json\" или \"csv\".
     * @param outPath Путь к выходному файлу.
     * @throws std::invalid_argument Если указан неподдерживаемый формат.
     * @throws std::runtime_error При ошибках записи.
     */
    void exportAll(const std::string& format, const std::string& outPath) const;

    /**
     * @brief Возвращает внутренний вектор задач.
     * @return const ссылка на вектор Task.
     */
    const std::vector<Task>& getAllTasks() const noexcept;

    /**
     * @brief Заменяет весь список задач (для Undo или после загрузки).
     * @param tasks Новый вектор задач.
     */
    void setAllTasks(const std::vector<Task>& tasks);

private:
    std::vector<Task> tasks_;  ///< Вектор всех задач.

    /**
     * @brief Ищет индекс задачи в векторе по ID.
     * @param id ID задачи.
     * @return Индекс в векторе или -1, если не найден.
     */
    int findIndexById(int id) const noexcept;

    /**
     * @brief Проверяет существование задачи по ID.
     * @param id ID задачи.
     * @throws std::runtime_error Если задача не найдена.
     */
    void ensureExists(int id) const;
};
