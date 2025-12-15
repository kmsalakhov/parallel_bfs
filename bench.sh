#!/bin/bash

# bench — запускает benchmark и сохраняет результат в указанный файл
# Использование: ./bench <filename>  → результат в benchmark_results/<filename>.json

set -e  # Выход при любой ошибке

if [ $# -ne 1 ]; then
    echo "Использование: $0 <filename>"
    echo "Пример: $0 cube300"
    exit 1
fi

FILENAME="$1"
OUTPUT_DIR="benchmark_results"
OUTPUT_PATH="$OUTPUT_DIR/${FILENAME}.json"

# Создаём директорию, если не существует
mkdir -p "$OUTPUT_DIR"

# Запускаем бенчмарк с нужными настройками
echo "Запуск бенчмарка..."
echo "  → Результат будет сохранён в: $OUTPUT_PATH"
echo "  → Используется PARLAY_NUM_THREADS=4"
echo

PARLAY_NUM_THREADS=4 ./build/bfs_benchmark \
  --benchmark_display_aggregates_only=true \
  --benchmark_out="$OUTPUT_PATH" \
  --benchmark_out_format=json

echo
echo "Готово! Результат записан в $OUTPUT_PATH"