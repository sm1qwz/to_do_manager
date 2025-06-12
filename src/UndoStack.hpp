#pragma once

#include "Task.hpp"
#include <vector>
#include <stack>

/**
 * @brief Стек для отмены (Undo) операций над списком задач.
 */
class UndoStack {
public:
    /**
     * @brief Сохраняет текущее состояние в стеке.
     * @param tasks Вектор задач на момент времени.
     */
    void pushState(const std::vector<Task>& tasks);

    /**
     * @brief Отменяет последнее действие.
     * @return Предыдущее сохранённое состояние списка задач.
     * @throws std::runtime_error Если нечего отменять.
     */
    std::vector<Task> undo();

    /**
     * @brief Проверяет, есть ли доступные для отмены состояния.
     * @return true, если стек не пуст.
     */
    bool canUndo() const noexcept;

private:
    std::stack<std::vector<Task>> history_; ///< История состояний списка задач.
};
