#include "CLIParser.hpp"
#include <stdexcept>
#include <algorithm>

CLIOptions CLIParser::parse(int argc, char* argv[]) {
    if (argc < 2) {
        throw std::runtime_error("No command provided");
    }
    CLIOptions opt;
    opt.command = argv[1];

    // По умолчанию
    opt.format = "json";
    opt.dataFilePath = "tasks.json";

    for (int i = 2; i < argc; ++i) {
        std::string token = argv[i];
        if (token.rfind("--", 0) == 0) {
            auto eqPos = token.find('=');
            if (eqPos != std::string::npos) {
                // Формат --key=value
                std::string key = token.substr(2, eqPos - 2);
                std::string val = token.substr(eqPos + 1);
                opt.args[key] = val;
            } else {
                // Формат --key value или флаги без значения
                std::string key = token.substr(2);
                // Флаги для list: --all, --done, --pending
                if (opt.command == "list" &&
                    (key == "all" || key == "done" || key == "pending")) {
                    opt.args["filter"] = key;
                }
                // Опции с аргументом через пробел
                else if ((opt.command == "export" && (key == "format" || key == "out"))
                      || key == "data-file" 
                      || key == "store-format" 
                      || (opt.command == "add" && (key == "due" || key == "tags"))
                      || (opt.command == "update-date" && key == "due")) {
                    if (i + 1 >= argc) {
                        throw std::runtime_error("Missing value for option: " + token);
                    }
                    opt.args[key] = argv[++i];
                }
                else {
                    throw std::runtime_error("Invalid option format: " + token);
                }
            }
        } else {
            // Позиционные аргументы
            if (opt.command == "add") {
                if (!opt.args.count("description")) {
                    opt.args["description"] = token;
                } else {
                    opt.args["description"] += " " + token;
                }
            } else if (opt.command == "remove" || opt.command == "done" || opt.command == "update-date") {
                if (!opt.args.count("id")) {
                    opt.args["id"] = token;
                }
            } else if (opt.command == "list") {
                // Позиционный фильтр
                opt.args["filter"] = token;
            } else if (opt.command == "export") {
                throw std::runtime_error("Unexpected positional argument for export: " + token);
            } else if (opt.command == "undo") {
                // Нет аргументов
            } else {
                throw std::runtime_error("Unknown command or invalid argument: " + token);
            }
        }
    }

    // Обработка глобальных опций
    if (opt.args.count("data-file")) {
        opt.dataFilePath = opt.args["data-file"];
    }
    if (opt.args.count("store-format")) {
        opt.format = opt.args["store-format"];
        if (opt.format != "json" && opt.format != "sqlite") {
            throw std::runtime_error("Unsupported storage format: " + opt.format);
        }
    }

    return opt;
}
