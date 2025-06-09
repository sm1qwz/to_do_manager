#include "CLIParser.hpp"
#include <stdexcept>
#include <algorithm>

/*
 Простейший алгоритм разбора:
 - Первый аргумент (после имени программы) — это подкоманда (add/remove/list/done/update-date/export/undo).
 - Любые ключи вида --key=value парсятся в map args.
 - Позиционные аргументы (например, описание или id) помещаются в args с фиксированными ключами:
   * Для add: args["description"]
   * Для remove, done, update-date: args["id"]
   * Для list: может быть args["filter"] (all|done|pending)
   * Для export: args["format"], args["out"]
 - --data-file и --format (для хранения) тоже берутся из args: args["data-file"], args["store-format"].
*/

CLIOptions CLIParser::parse(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("No command provided");
    }
    CLIOptions opt;
    opt.command = argv[1];

    // По умолчанию
    opt.format = "json";
    opt.dataFilePath = "tasks.json";

    // Начнём с argv[2]
    for (int i = 2; i < argc; ++i) {
        std::string token = argv[i];
        if (token.rfind("--", 0) == 0) {
            // Ключ вида --key=value
            auto eqPos = token.find('=');
            if (eqPos == std::string::npos) {
                throw std::runtime_error("Invalid option format: " + token);
            }
            std::string key = token.substr(2, eqPos - 2);
            std::string val = token.substr(eqPos + 1);
            opt.args[key] = val;
        } else {
            // Позиционный аргумент. Интерпретируем в зависимости от команды
            if (opt.command == "add") {
                // Если описание может содержать пробелы, мы могли бы брать оставшуюся часть всю сразу
                if (!opt.args.count("description")) {
                    opt.args["description"] = token;
                } else {
                    opt.args["description"] += " " + token;
                }
            } else if (opt.command == "remove" || opt.command == "done" || opt.command == "update-date") {
                if (!opt.args.count("id")) {
                    opt.args["id"] = token;
                } else {
                    // игнор или ошибка, если больше одного id
                }
            } else if (opt.command == "list") {
                // Возможные фильтры: all, done, pending
                opt.args["filter"] = token;
            } else if (opt.command == "export") {
                // Позиционно: format и out, но удобнее требовать --format и --out
                // Поэтому, если позиционно, можно бросить ошибку
                throw std::runtime_error("Unexpected positional argument for export: " + token);
            } else if (opt.command == "undo") {
                // У undo нет позиционных аргументов
                // Игнорировать лишнее
            } else {
                throw std::runtime_error("Unknown command or invalid argument: " + token);
            }
        }
    }

    // Обработка значений --data-file и --format (для хранилища)
    if (opt.args.count("data-file")) {
        opt.dataFilePath = opt.args.at("data-file");
    }
    if (opt.args.count("store-format")) {
        opt.format = opt.args.at("store-format");
        if (opt.format != "json" && opt.format != "sqlite") {
            throw std::runtime_error("Unsupported storage format: " + opt.format);
        }
    }

    return opt;
}
