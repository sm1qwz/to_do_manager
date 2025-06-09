#include "UndoStack.hpp"
#include <stdexcept>

void UndoStack::pushState(const std::vector<Task>& tasks) {
    history_.push(tasks);
}

std::vector<Task> UndoStack::undo() {
    if (history_.empty()) {
        throw std::runtime_error("Nothing to undo");
    }
    auto prev = history_.top();
    history_.pop();
    return prev;
}

bool UndoStack::canUndo() const noexcept {
    return !history_.empty();
}
