#pragma once

#include <string>
#include <vector>
#include <optional>
#include <nlohmann/json.hpp>   
/**
 * @brief Класс Task описывает одну задачу в списке.
 */
class Task {
public:
    /**
     * @brief Конструктор без dueDate. ID генерируется автоматически.
     * @param description Описание задачи
     * @param tags Список тегов (опционально)
     */
    Task(const std::string& description,
         const std::vector<std::string>& tags = {});

    /**
     * @brief Конструктор с dueDate. ID генерируется автоматически.
     * @param description Описание задачи
     * @param dueDate Дата дедлайна (формат YYYY-MM-DD)
     * @param tags Список тегов (опционально)
     */
    Task(const std::string& description,
         const std::string& dueDate,
         const std::vector<std::string>& tags = {});

    // Геттеры/сеттеры
    int getId() const noexcept;
    const std::string& getDescription() const noexcept;
    const std::optional<std::string>& getDueDate() const noexcept;
    bool isDone() const noexcept;
    const std::vector<std::string>& getTags() const noexcept;

    void setDescription(const std::string& desc);
    void setDueDate(const std::string& dueDate);
    void markDone();
    void addTag(const std::string& tag);
    void removeTag(const std::string& tag);

    /**
     * @brief Сериализует задачу в JSON-объект (для хранения)
     */
    nlohmann::json toJson() const;

    /**
     * @brief Создаёт Task из JSON-объекта
     */
    static Task fromJson(const nlohmann::json& j);

private:
    int id_;                             ///< Уникальный идентификатор
    std::string description_;            ///< Текст описания
    std::optional<std::string> dueDate_; ///< Дата дедлайна (format YYYY-MM-DD)
    bool done_ = false;                  ///< Статус выполнения
    std::vector<std::string> tags_;      ///< Теги/категории

    static int nextId_;                  ///< Статический счётчик для генерации уникальных ID
};
