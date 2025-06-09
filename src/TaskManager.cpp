#include "TaskManager.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip> // для CSV

int TaskManager::addTask(const std::string& description,
                         const std::optional<std::string>& dueDate,
                         const std::vector<std::string>& tags) {
    Task t(dueDate.has_value() ? Task(description, *dueDate, tags)
                               : Task(description, tags));
    tasks_.push_back(t);
    return t.getId();
}

void TaskManager::removeTask(int id) {
    ensureExists(id);
    int idx = findIndexById(id);
    tasks_.erase(tasks_.begin() + idx);
}

void TaskManager::markDone(int id) {
    ensureExists(id);
    int idx = findIndexById(id);
    tasks_[idx].markDone();
}

std::vector<Task> TaskManager::listTasks(const std::optional<bool>& showDone) const {
    if (!showDone.has_value()) {
        return tasks_;
    }
    std::vector<Task> result;
    for (const auto& t : tasks_) {
        if (t.isDone() == *showDone) {
            result.push_back(t);
        }
    }
    return result;
}

std::vector<Task> TaskManager::searchByDescription(const std::string& substr) const {
    std::string lowerSub = substr;
    std::transform(lowerSub.begin(), lowerSub.end(), lowerSub.begin(), ::tolower);
    std::vector<Task> result;
    for (const auto& t : tasks_) {
        std::string desc = t.getDescription();
        std::transform(desc.begin(), desc.end(), desc.begin(), ::tolower);
        if (desc.find(lowerSub) != std::string::npos) {
            result.push_back(t);
        }
    }
    return result;
}

void TaskManager::updateDueDate(int id, const std::string& newDueDate) {
    ensureExists(id);
    int idx = findIndexById(id);
    tasks_[idx].setDueDate(newDueDate);
}

void TaskManager::addTag(int id, const std::string& tag) {
    ensureExists(id);
    int idx = findIndexById(id);
    tasks_[idx].addTag(tag);
}

void TaskManager::removeTag(int id, const std::string& tag) {
    ensureExists(id);
    int idx = findIndexById(id);
    tasks_[idx].removeTag(tag);
}

void TaskManager::exportAll(const std::string& format, const std::string& outPath) const {
    if (format == "json") {
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& t : tasks_) {
            arr.push_back(t.toJson());
        }
        std::ofstream ofs(outPath);
        if (!ofs) {
            throw std::runtime_error("Cannot open file for export: " + outPath);
        }
        ofs << std::setw(4) << arr;
    } else if (format == "csv") {
        std::ofstream ofs(outPath);
        if (!ofs) {
            throw std::runtime_error("Cannot open file for export: " + outPath);
        }
        // Заголовок CSV
        ofs << "id,description,dueDate,done,tags\n";
        for (const auto& t : tasks_) {
            ofs << t.getId() << ",";
            // Экранируем запятые, если нужно (упрощённый вариант)
            std::string desc = t.getDescription();
            if (desc.find(',') != std::string::npos) {
                ofs << "\"" << desc << "\"";
            } else {
                ofs << desc;
            }
            ofs << ",";
            if (t.getDueDate().has_value()) {
                ofs << t.getDueDate().value();
            }
            ofs << ",";
            ofs << (t.isDone() ? "1" : "0") << ",";
            // Теги через точку с запятой
            const auto& tags = t.getTags();
            for (size_t i = 0; i < tags.size(); ++i) {
                ofs << tags[i];
                if (i + 1 < tags.size()) ofs << ";";
            }
            ofs << "\n";
        }
    } else {
        throw std::invalid_argument("Unsupported export format: " + format);
    }
}

const std::vector<Task>& TaskManager::getAllTasks() const noexcept {
    return tasks_;
}

void TaskManager::setAllTasks(const std::vector<Task>& tasks) {
    tasks_ = tasks;
}

int TaskManager::findIndexById(int id) const noexcept {
    for (int i = 0; i < static_cast<int>(tasks_.size()); ++i) {
        if (tasks_[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

void TaskManager::ensureExists(int id) const {
    if (findIndexById(id) == -1) {
        throw std::runtime_error("Task with id " + std::to_string(id) + " not found");
    }
}
