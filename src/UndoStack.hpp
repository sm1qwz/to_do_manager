#pragma once

#include "Task.hpp"
#include <vector>
#include <stack>

/**
 * @brief UndoStack хранит историю изменений в виде вектора списков задач.
 */
class UndoStack {
public:
    /**
     * @brief Сохраняет текущее состояние в стеке изменений.
     * @param tasks копия вектора задач
     */
    void pushState(const std::vector<Task>& tasks);

    /**
     * @brief Отменяет последнее действие, возвращая предыдущий вектор задач.
     * @return предыдущее состояние (стек «сдвинут»)
     * @throws std::runtime_error, если стек пуст (некуда отменять)
     */
    std::vector<Task> undo();

    /**
     * @brief Проверяет, есть ли куда откатываться
     */
    bool canUndo() const noexcept;

private:
    std::stack<std::vector<Task>> history_;
};
