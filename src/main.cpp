#include <iostream>
#include "CLIParser.hpp"
#include "TaskManager.hpp"
#include "Storage.hpp"
#include "UndoStack.hpp"
#include "Logger.hpp"

int main(int argc, char* argv[]) {
    try {
        // 1) Парсим CLI
        CLIOptions opts = CLIParser::parse(argc, argv);

        // 2) Инициализируем Storage (из --data-file и store-format)
        Storage storage(opts.dataFilePath, opts.format);

        // 3) Загружаем текущий список задач
        std::vector<Task> loadedTasks = storage.load();

        // 4) Инициализируем менеджер и загрузим в него
        TaskManager manager;
        manager.setAllTasks(loadedTasks);

        // 5) Инициализируем UndoStack и Logger
        UndoStack undoStack;
        Logger& logger = Logger::instance("history.log");

        // 6) В зависимости от команды выполняем действия
        const std::string& cmd = opts.command;
        if (cmd == "add") {
            // Сохраняем состояние для undo
            undoStack.pushState(manager.getAllTasks());

            std::string desc = opts.args.at("description");
            std::optional<std::string> due = std::nullopt;
            if (opts.args.count("due")) {
                due = opts.args.at("due");
            }
            std::vector<std::string> tags;
            if (opts.args.count("tags")) {
                // Теги через запятую: "work,urgent"
                std::string tagsStr = opts.args.at("tags");
                size_t pos = 0;
                while (true) {
                    auto comma = tagsStr.find(',', pos);
                    if (comma == std::string::npos) {
                        tags.push_back(tagsStr.substr(pos));
                        break;
                    }
                    tags.push_back(tagsStr.substr(pos, comma - pos));
                    pos = comma + 1;
                }
            }
            int newId = manager.addTask(desc, due, tags);
            storage.save(manager.getAllTasks());
            logger.log("ADD id=" + std::to_string(newId) + " description=\"" + desc + "\""
                       + (due ? (" due=" + *due) : "")
                       + (tags.empty() ? "" : " tags=[" + opts.args.at("tags") + "]"));
            std::cout << "Task added with id " << newId << "\n";
        } else if (cmd == "remove") {
            int id = std::stoi(opts.args.at("id"));
            undoStack.pushState(manager.getAllTasks());
            manager.removeTask(id);
            storage.save(manager.getAllTasks());
            logger.log("REMOVE id=" + std::to_string(id));
            std::cout << "Task " << id << " removed\n";
        } else if (cmd == "done") {
            int id = std::stoi(opts.args.at("id"));
            undoStack.pushState(manager.getAllTasks());
            manager.markDone(id);
            storage.save(manager.getAllTasks());
            logger.log("DONE id=" + std::to_string(id));
            std::cout << "Task " << id << " marked done\n";
        } else if (cmd == "list") {
            std::optional<bool> filter = std::nullopt;
            if (opts.args.count("filter")) {
                std::string f = opts.args.at("filter");
                if (f == "all") {
                    filter = std::nullopt;
                } else if (f == "done") {
                    filter = true;
                } else if (f == "pending") {
                    filter = false;
                } else {
                    throw std::runtime_error("Unknown filter: " + f);
                }
            }
            auto tasksToShow = manager.listTasks(filter);
            for (const auto& t : tasksToShow) {
                std::cout << "[" << t.getId() << "] "
                          << (t.isDone() ? "[x] " : "[ ] ")
                          << t.getDescription();
                if (t.getDueDate().has_value()) {
                    std::cout << " (due " << *t.getDueDate() << ")";
                }
                if (!t.getTags().empty()) {
                    std::cout << " {";
                    const auto& tg = t.getTags();
                    for (size_t i = 0; i < tg.size(); ++i) {
                        std::cout << tg[i];
                        if (i + 1 < tg.size()) std::cout << ",";
                    }
                    std::cout << "}";
                }
                std::cout << "\n";
            }
        } else if (cmd == "search") {
            std::string substr = opts.args.at("query");
            auto found = manager.searchByDescription(substr);
            for (const auto& t : found) {
                std::cout << "[" << t.getId() << "] "
                          << (t.isDone() ? "[x] " : "[ ] ")
                          << t.getDescription() << "\n";
            }
        } else if (cmd == "update-date") {
            int id = std::stoi(opts.args.at("id"));
            std::string newDue = opts.args.at("due");
            undoStack.pushState(manager.getAllTasks());
            manager.updateDueDate(id, newDue);
            storage.save(manager.getAllTasks());
            logger.log("UPDATE-DATE id=" + std::to_string(id) + " due=" + newDue);
            std::cout << "Task " << id << " due-date updated to " << newDue << "\n";
        } else if (cmd == "export") {
            std::string fmt = opts.args.at("format");
            std::string out = opts.args.at("out");
            manager.exportAll(fmt, out);
            logger.log("EXPORT format=" + fmt + " out=" + out);
            std::cout << "Exported to " << out << "\n";
        } else if (cmd == "undo") {
            if (!undoStack.canUndo()) {
                std::cout << "Nothing to undo\n";
            } else {
                auto prev = undoStack.undo();
                manager.setAllTasks(prev);
                storage.save(manager.getAllTasks());
                logger.log("UNDO");
                std::cout << "Last action undone\n";
            }
        } else {
            std::cout << "Unknown command: " << cmd << "\n";
            return 1;
        }

        return 0;
    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }
}
