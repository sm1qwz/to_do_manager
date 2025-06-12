# To-Do Manager (Console Task Manager)

**Консольное приложение** для создания, хранения и управления списком дел в локальном файле (JSON или SQLite).

---

## Возможности

* Добавление задач с описанием, тегами и необязательным дедлайном (формат **YYYY-MM-DD**).
* Удаление задачи по идентификатору.
* Отметка задачи как выполненной.
* Просмотр списка задач с фильтрами:

  * `--all` — все задачи (по умолчанию).
  * `--done` — только выполненные.
  * `--pending` — только активные.
* Поиск по подстроке в описании: `search <query>`.
* Присвоение и удаление тегов.
* Обновление даты дедлайна: `update-date <id> --due YYYY-MM-DD`.
* Экспорт задач в **JSON** или **CSV**: `export --format <json|csv> --out <path>`.
* Отмена последнего действия (`undo`).
* Логирование операций (`add`, `remove`, `done`, `update-date`, `export`, `undo`) в файл `history.log`.
* Поддержка двух форматов хранения данных:

  * **JSON** (по умолчанию).
  * **SQLite** (при сборке с флагом `--store-format=sqlite`).

---

## Установка и сборка

1. Клонируйте репозиторий:

   ```bash
   git clone https://github.com/yourusername/to_do_manager.git
   cd to_do_manager
   ```
2. Подготовьте зависимости:

   * **nlohmann/json**: подтягивается автоматически через CMake (`FetchContent`).
   * **SQLite** (опционально): установите пакет разработчика:

     * Ubuntu/Debian:

       ```bash
       sudo apt update && sudo apt install libsqlite3-dev
       ```
     * macOS (Homebrew):

       ```bash
       brew install sqlite
       ```
3. Соберите проект с помощью CMake:

   ```bash
   mkdir build && cd build
   cmake ..
   make
   ```

---

## Запуск

Пример базовых команд (из каталога `build`):

* **Добавить задачу**:

  ```bash
  ./ToDoManager add "Купить хлеб" --due=2025-06-10 --tags=shopping,urgent
  ```
* **Посмотреть все задачи**:

  ```bash
  ./ToDoManager list --all
  ```
* **Посмотреть только выполненные**:

  ```bash
  ./ToDoManager list --done
  ```
* **Отметить задачу выполненной**:

  ```bash
  ./ToDoManager done 3
  ```
* **Удалить задачу**:

  ```bash
  ./ToDoManager remove 5
  ```
* **Поиск по описанию**:

  ```bash
  ./ToDoManager search "bread"
  ```
* **Обновить дедлайн**:

  ```bash
  ./ToDoManager update-date 2 --due=2025-07-01
  ```
* **Экспорт в CSV**:

  ```bash
  ./ToDoManager export --format csv --out tasks.csv
  ```
* **Отмена последнего действия**:

  ```bash
  ./ToDoManager undo
  ```

> По умолчанию данные хранятся в `tasks.json` в текущем каталоге. Можно задать свой файл:
>
> ```bash
> ./ToDoManager add "Test" --data-file=/path/to/my_tasks.json
> ```
>
> Для SQLite: `--store-format=sqlite`.

---

## Тестирование

1. Перейдите в каталог `build` и выполните:

   ```bash
   ctest --output-on-failure
   ```
2. Все тесты написаны с использованием **Google Test** и покрывают основные функции.

---

## Документация

* Документация в формате **Doxygen** расположена в каталоге `docs/`. Для генерации:

  ```bash
  cd docs
  doxygen -g Doxyfile
  ```
* Сгенерированные HTML-файлы будут в `docs/html/`.

---

## Кодировка и форматирование

* Все исходные файлы в кодировке **UTF-8**.
* Код форматируется через **clang-format** по стилю `Google`. Чтобы применить форматирование:

  ```bash
  clang-format -i -style=file src/*.cpp src/*.hpp tests/*.cpp
  ```

---

## Лицензия

Проект распространяется под лицензией MIT. См. файл `LICENSE`.
