# examples/add_example.sh
#!/usr/bin/env bash
#
# Пример: добавление задачи в список.
# Использует ./ToDoManager из build-папки и по умолчанию файл tasks.json.
#

APP="../build/ToDoManager"
DATA_FILE="tasks.json"

# Добавляем задачу "Купить молоко" с дедлайном и двумя тегами
"$APP" add "Купить молоко" \
    --due=2025-06-15 \
    --tags=shopping,dairy \
    --data-file="$DATA_FILE"
