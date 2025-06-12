#include "Storage.hpp"
#include <fstream>
#include <stdexcept>
#include <sstream>           // для разбора строк тегов
#include <sqlite3.h>        // если используем SQLite
#include <json.hpp>

Storage::Storage(const std::string& dataFilePath, const std::string& format)
    : dataFilePath_(dataFilePath), format_(format) {}

std::vector<Task> Storage::load() {
    if (format_ == "json") {
        std::ifstream ifs(dataFilePath_);
        if (!ifs) {
            // Файл не существует или не открылся — возвращаем пустой список
            return {};
        }
        nlohmann::json arr;
        ifs >> arr;
        std::vector<Task> tasks;
        for (const auto& item : arr) {
            tasks.push_back(Task::fromJson(item));
        }
        return tasks;
    } else if (format_ == "sqlite") {
        sqlite3* db = nullptr;
        if (sqlite3_open(dataFilePath_.c_str(), &db) != SQLITE_OK) {
            throw std::runtime_error("Cannot open SQLite database: " + dataFilePath_);
        }
        std::vector<Task> tasks;
        const char* query = R"(
            SELECT id, description, dueDate, done, tags
            FROM tasks;
        )";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            throw std::runtime_error("Failed to prepare SQLite statement");
        }
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            nlohmann::json j;
            j["id"] = sqlite3_column_int(stmt, 0);
            j["description"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            j["done"] = sqlite3_column_int(stmt, 3) == 1;
            if (sqlite3_column_text(stmt, 2)) {
                j["dueDate"] = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            }
            // Разбор тегов из строки вида "tag1;tag2;tag3"
            if (sqlite3_column_text(stmt, 4)) {
                std::string tagsStr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
                std::vector<std::string> tagsVec;
                std::istringstream ss(tagsStr);
                std::string tag;
                while (std::getline(ss, tag, ';')) {
                    if (!tag.empty()) tagsVec.push_back(tag);
                }
                j["tags"] = tagsVec;
            }
            tasks.push_back(Task::fromJson(j));
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return tasks;
    } else {
        throw std::invalid_argument("Unsupported format in Storage: " + format_);
    }
}

void Storage::save(const std::vector<Task>& tasks) {
    if (format_ == "json") {
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& t : tasks) {
            arr.push_back(t.toJson());
        }
        std::ofstream ofs(dataFilePath_);
        if (!ofs) {
            throw std::runtime_error("Cannot open file for saving: " + dataFilePath_);
        }
        ofs << std::setw(4) << arr;
    } else if (format_ == "sqlite") {
        sqlite3* db = nullptr;
        if (sqlite3_open(dataFilePath_.c_str(), &db) != SQLITE_OK) {
            throw std::runtime_error("Cannot open SQLite database: " + dataFilePath_);
        }
        const char* sqlCreate = R"(
            CREATE TABLE IF NOT EXISTS tasks (
                id INTEGER PRIMARY KEY,
                description TEXT NOT NULL,
                dueDate TEXT,
                done INTEGER,
                tags TEXT
            );
        )";
        char* errmsg = nullptr;
        if (sqlite3_exec(db, sqlCreate, nullptr, nullptr, &errmsg) != SQLITE_OK) {
            std::string err = errmsg;
            sqlite3_free(errmsg);
            sqlite3_close(db);
            throw std::runtime_error("SQLite error: " + err);
        }
        // Очищаем таблицу для перезаписи
        if (sqlite3_exec(db, "DELETE FROM tasks;", nullptr, nullptr, &errmsg) != SQLITE_OK) {
            std::string err = errmsg;
            sqlite3_free(errmsg);
            sqlite3_close(db);
            throw std::runtime_error("SQLite error: " + err);
        }
        const char* sqlIns = R"(
            INSERT INTO tasks (id, description, dueDate, done, tags)
            VALUES (?, ?, ?, ?, ?);
        )";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, sqlIns, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            throw std::runtime_error("Failed to prepare SQLite insert statement");
        }
        for (const auto& t : tasks) {
            sqlite3_bind_int(stmt, 1, t.getId());
            sqlite3_bind_text(stmt, 2, t.getDescription().c_str(), -1, SQLITE_TRANSIENT);
            if (t.getDueDate().has_value()) {
                sqlite3_bind_text(stmt, 3, t.getDueDate()->c_str(), -1, SQLITE_TRANSIENT);
            } else {
                sqlite3_bind_null(stmt, 3);
            }
            sqlite3_bind_int(stmt, 4, t.isDone() ? 1 : 0);
            // Собираем теги через ';'
            const auto& tags = t.getTags();
            std::string tagsJoined;
            for (size_t i = 0; i < tags.size(); ++i) {
                tagsJoined += tags[i];
                if (i + 1 < tags.size()) tagsJoined += ";";
            }
            sqlite3_bind_text(stmt, 5, tagsJoined.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmt) != SQLITE_DONE) {
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                throw std::runtime_error("Failed to insert task into SQLite");
            }
            sqlite3_reset(stmt);
        }
        sqlite3_finalize(stmt);
        sqlite3_close(db);
    } else {
        throw std::invalid_argument("Unsupported format in Storage: " + format_);
    }
}
