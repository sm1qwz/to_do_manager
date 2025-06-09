#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>

/**
 * @brief Структура, в которой хранятся результаты разбора аргументов CLI.
 */
struct CLIOptions {
    std::string command;                       ///< add, remove, list, done, update-date, export, undo
    std::string dataFilePath;                  ///< путь к файлу (из --data-file)
    std::string format;                        ///< формат хранения (json или sqlite)
    std::unordered_map<std::string, std::string> args; ///< прочие аргументы, например: description, id, due, format, out, filter
};

/**
 * @brief Класс для разбора аргументов командной строки.
 */
class CLIParser {
public:
    /**
     * @brief Разбирает argc/argv и заполняет структуру CLIOptions.
     * @param argc количество аргументов
     * @param argv массив аргументов
     * @return заполненную структуру с командой и опциями
     */
    static CLIOptions parse(int argc, char* argv[]);
};
