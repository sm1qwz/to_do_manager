#include "Task.hpp"
#include <stdexcept>

int Task::nextId_ = 1;

Task::Task(const std::string& description, const std::vector<std::string>& tags)
    : id_(nextId_++), description_(description), tags_(tags) {}

Task::Task(const std::string& description, const std::string& dueDate, const std::vector<std::string>& tags)
    : id_(nextId_++), description_(description), dueDate_(dueDate), tags_(tags) {}

int Task::getId() const noexcept {
    return id_;
}

const std::string& Task::getDescription() const noexcept {
    return description_;
}

const std::optional<std::string>& Task::getDueDate() const noexcept {
    return dueDate_;
}

bool Task::isDone() const noexcept {
    return done_;
}

const std::vector<std::string>& Task::getTags() const noexcept {
    return tags_;
}

void Task::setDescription(const std::string& desc) {
    description_ = desc;
}

void Task::setDueDate(const std::string& dueDate) {
    dueDate_ = dueDate;
}

void Task::markDone() {
    done_ = true;
}

void Task::addTag(const std::string& tag) {
    tags_.push_back(tag);
}

void Task::removeTag(const std::string& tag) {
    tags_.erase(std::remove(tags_.begin(), tags_.end(), tag), tags_.end());
}

nlohmann::json Task::toJson() const {
    nlohmann::json j;
    j["id"] = id_;
    j["description"] = description_;
    j["done"] = done_;
    if (dueDate_) {
        j["dueDate"] = *dueDate_;
    }
    if (!tags_.empty()) {
        j["tags"] = tags_;
    }
    return j;
}

Task Task::fromJson(const nlohmann::json& j) {
    Task t(j.at("description").get<std::string>());
    t.id_ = j.at("id").get<int>();
    t.done_ = j.at("done").get<bool>();
    if (j.contains("dueDate")) {
        t.dueDate_ = j.at("dueDate").get<std::string>();
    }
    if (j.contains("tags")) {
        t.tags_ = j.at("tags").get<std::vector<std::string>>();
    }
    // Убедимся, что nextId_ > всех прочитанных id
    if (t.id_ >= Task::nextId_) {
        Task::nextId_ = t.id_ + 1;
    }
    return t;
}
