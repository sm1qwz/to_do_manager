#pragma once

#include <string>
#include <vector>
#include <optional>
#if __has_include(<nlohmann/json.hpp>)
#include <nlohmann/json.hpp>
#else
// Если библиотека nlohmann/json установлена локально, укажите путь:
#include "json.hpp"
#endif

/**
 * @brief Класс, описывающий одну задачу.
 */
class Task {
public:
    /**
     * @brief Создаёт задачу без дедлайна.
     * @param description Описание задачи.
     * @param tags Список тегов (по умолчанию пустой).
     */
    Task(const std::string& description,
         const std::vector<std::string>& tags = {});

    /**
     * @brief Создаёт задачу с дедлайном.
     * @param description Описание задачи.
     * @param dueDate Дата завершения в формате YYYY-MM-DD.
     * @param tags Список тегов (по умолчанию пустой).
     */
    Task(const std::string& description,
         const std::string& dueDate,
         const std::vector<std::string>& tags = {});

    /**
     * @brief Возвращает уникальный идентификатор задачи.
     * @return Целочисленный ID.
     */
    int getId() const noexcept;

    /**
     * @brief Возвращает описание задачи.
     * @return Константная ссылка на строку с описанием.
     */
    const std::string& getDescription() const noexcept;

    /**
     * @brief Возвращает дедлайн задачи (если установлен).
     * @return optional со строкой YYYY-MM-DD или std::nullopt.
     */
    const std::optional<std::string>& getDueDate() const noexcept;

    /**
     * @brief Проверяет, выполнена ли задача.
     * @return true, если задача помечена выполненной.
     */
    bool isDone() const noexcept;

    /**
     * @brief Возвращает список тегов задачи.
     * @return Вектор строк-тегов.
     */
    const std::vector<std::string>& getTags() const noexcept;

    /**
     * @brief Изменяет текст описания задачи.
     * @param desc Новое описание.
     */
    void setDescription(const std::string& desc);

    /**
     * @brief Устанавливает или изменяет дедлайн задачи.
     * @param dueDate Новая дата дедлайна (YYYY-MM-DD).
     */
    void setDueDate(const std::string& dueDate);

    /**
     * @brief Помечает задачу как выполненную.
     */
    void markDone();

    /**
     * @brief Добавляет тег к задаче, если он ещё не добавлен.
     * @param tag Метка для добавления.
     */
    void addTag(const std::string& tag);

    /**
     * @brief Удаляет тег из задачи, если он присутствует.
     * @param tag Метка для удаления.
     */
    void removeTag(const std::string& tag);

    /**
     * @brief Сериализует задачу в JSON-объект.
     * @return nlohmann::json с полями id, description, done, dueDate и tags.
     */
    nlohmann::json toJson() const;

    /**
     * @brief Создаёт объект Task из JSON-объекта.
     * @param j JSON-объект с теми же полями, что были в toJson().
     * @return Восстановленный объект Task.
     * @throws std::out_of_range Если в JSON отсутствуют обязательные поля id, description или done.
     */
    static Task fromJson(const nlohmann::json& j);

private:
    int id_;                             ///< Уникальный идентификатор задачи.
    std::string description_;            ///< Описание задачи.
    std::optional<std::string> dueDate_; ///< Дата дедлайна (формат YYYY-MM-DD).
    bool done_ = false;                  ///< Статус выполнения.
    std::vector<std::string> tags_;      ///< Список меток (тегов).

    static int nextId_;                  ///< Генератор уникальных ID.
};
