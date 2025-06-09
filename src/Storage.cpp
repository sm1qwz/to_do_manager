#include "Storage.hpp"
#include <fstream>
#include <stdexcept>
#include <sqlite3.h>        // если используем SQLite
#include "nlohmann/json.hpp"

Storage::Storage(const std::string& dataFilePath, const std::string& format)
    : dataFilePath_(dataFilePath), format_(format) {}

std::vector<Task> Storage::load() {
    if (format_ == "json") {
        std::ifstream ifs(dataFilePath_);
        if (!ifs) {
            // Если файл не существует или не открылся, возвращаем пустой вектор
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
        const char* sql = "CREATE TABLE IF NOT EXISTS tasks ("
                          "id INTEGER PRIMARY KEY, "
                          "description TEXT NOT NULL, "
                          "dueDate TEXT, "
                          "done INTEGER, "
                          "tags TEXT);";
        char* errmsg = nullptr;
        if (sqlite3_exec(db, sql, 0, 0, &errmsg) != SQLITE_OK) {
            std::string err = errmsg;
            sqlite3_free(errmsg);
            sqlite3_close(db);
            throw std::runtime_error("SQLite error: " + err);
        }
        const char* query = "SELECT id, description, dueDate, done, tags FROM tasks;";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, query, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            throw std::runtime_error("Failed to prepare SQLite statement");
        }
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            int id = sqlite3_column_int(stmt, 0);
            const unsigned char* descText = sqlite3_column_text(stmt, 1);
            const unsigned char* dueText = sqlite3_column_text(stmt, 2);
            int doneInt = sqlite3_column_int(stmt, 3);
            const unsigned char* tagsText = sqlite3_column_text(stmt, 4);

            Task t(std::string(reinterpret_cast<const char*>(descText)));
            t.markDone(); // в конструкторе done=false, но далее ниже перезапишем
            t = Task::fromJson(nlohmann::json::object()); // чтобы не слomать логику; ниже переустановим поля
            // Чтобы корректно восстановить всё, проще один раз взять JSON-строку из таблицы (если храним так)
            // Или, если храним в отдельных колонках, нужно сеттеры:
            t = Task(std::string(reinterpret_cast<const char*>(descText)));
            t.setDescription(reinterpret_cast<const char*>(descText));
            if (dueText) {
                t.setDueDate(reinterpret_cast<const char*>(dueText));
            }
            if (doneInt == 1) {
                t.markDone();
            }
            if (tagsText) {
                std::string tagsStr(reinterpret_cast<const char*>(tagsText));
                // Разбить по ';'
                std::vector<std::string> tagsVec;
                std::istringstream ss(tagsStr);
                std::string item;
                while (std::getline(ss, item, ';')) {
                    if (!item.empty())
                        tagsVec.push_back(item);
                }
                for (const auto& tg : tagsVec) {
                    t.addTag(tg);
                }
            }
            // Устанавливаем id вручную (Task::nextId_ внутри вызова fromJson позаботится)
            // Но в нашем конструкторе id генерируется автоматически, поэтому
            // придётся сделать метод в Task для установки id напрямую,
            // либо хранить JSON внутри SQLite, чтобы не дублировать логику.
            // Упрощённый вариант: хранить весь объект JSON в колонке и вызывать Task::fromJson.
            nlohmann::json j = {
                {"id", id},
                {"description", reinterpret_cast<const char*>(descText)},
                {"done", doneInt == 1}
            };
            if (dueText) {
                j["dueDate"] = std::string(reinterpret_cast<const char*>(dueText));
            }
            if (tagsText) {
                // собираем JSON-массив из tagsVec
                j["tags"] = nlohmann::json(tagsText); // если там JSON-строка
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
        // Создаём таблицу, если её нет
        const char* sqlCreate = "CREATE TABLE IF NOT EXISTS tasks ("
                                "id INTEGER PRIMARY KEY, "
                                "description TEXT NOT NULL, "
                                "dueDate TEXT, "
                                "done INTEGER, "
                                "tags TEXT);";
        char* errmsg = nullptr;
        if (sqlite3_exec(db, sqlCreate, 0, 0, &errmsg) != SQLITE_OK) {
            std::string err = errmsg;
            sqlite3_free(errmsg);
            sqlite3_close(db);
            throw std::runtime_error("SQLite error: " + err);
        }
        // Очищаем все строки (если хотим перезаписать)
        const char* sqlDel = "DELETE FROM tasks;";
        if (sqlite3_exec(db, sqlDel, 0, 0, &errmsg) != SQLITE_OK) {
            std::string err = errmsg;
            sqlite3_free(errmsg);
            sqlite3_close(db);
            throw std::runtime_error("SQLite error: " + err);
        }
        // Подготавливаем вставку
        const char* sqlIns = "INSERT INTO tasks(id, description, dueDate, done, tags) VALUES (?, ?, ?, ?, ?);";
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
            // Теги в виде JSON-строки или «;»-разделённых
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
