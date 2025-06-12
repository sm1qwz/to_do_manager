# examples/list_example.sh
#!/usr/bin/env bash
#
# Пример: вывод всех задач из списка.
# Параметры:
#   --all     — показать все (по умолчанию)
#   --done    — только выполненные
#   --pending — только незавершённые
#

APP="../build/ToDoManager"
DATA_FILE="tasks.json"

# Вывести все задачи
"$APP" list --all --data-file="$DATA_FILE"

# (для примера можно раскомментировать другую опцию)
# "$APP" list --done --data-file="$DATA_FILE"
# "$APP" list --pending --data-file="$DATA_FILE"
