//! Требуется разработать программу, осуществляющую ввод пар «ключ-значение», их упорядочивание по возрастанию ключа указанным алгоритмом сортировки за линейное время и вывод отсортированной последовательности.
//! Вариант задания определяется типом ключа (и соответствующим ему методом сортировки) и типом значения:
//! Поразрядная сортировка.
//!
//! Тип ключа: даты в формате DD.MM.YYYY, например 1.1.1, 1.9.2009, 01.09.2009, 31.12.2009.
//!
//! Тип значения: строки фиксированной длины 64 символа, во входных данных могут встретиться строки меньшей длины, при этом строка дополняется до 64-х нулевыми символами, которые не выводятся на экран.
//!
//! Формат ввода
//! На каждой непустой строке входного файла располагается пара «ключ-значение», в которой ключ указан согласно заданию, затем следует знак табуляции и указано соответствующее значение.
//!
//! Формат вывода
//! Выходные данные состоят из тех же строк, что и входные, за исключением пустых и порядка следования.

#include <stdio.h>
#define _STRING_IMPLEMENTATION
#include "string.h"


uint64_t pair_count = 0;
uint64_t current_capacity = 2;


typedef struct {
    int key; // key = year * 10000 + month * 100 + day, max key is 99991231 ()
    string value;
} key_value_pair;


void radix_sort(key_value_pair* pairs) {
    uint64_t *sorting_array = calloc(256, sizeof(int));

    for (uint64_t binary_shift = 0; binary_shift < sizeof(int); binary_shift += 1) {
        key_value_pair *output_array = calloc(pair_count, sizeof(key_value_pair));
        int byte_mask = 0b11111111 << binary_shift;

        printf("%"PRIu64" bin shift => byte_mask is %d\n", binary_shift, byte_mask);

        for (uint64_t pair_index = 0; pair_index < pair_count; ++pair_index) {
            int byte = (pairs[pair_index].key & byte_mask) >> binary_shift;
            printf("@%"PRIu64" byte is %d\n", pair_index, byte);
            ++sorting_array[byte];
        }

        for (int byte = 1; byte <= 0b11111111; ++byte) sorting_array[byte] += sorting_array[byte - 1];

        for (uint64_t pair_index = pair_count + 1; pair_index - 1 > 0; --pair_index) {
            int byte = (pairs[pair_index - 2].key & byte_mask) >> binary_shift;
            output_array[sorting_array[byte]] = pairs[pair_index - 2];
            --sorting_array[byte];
        }

        key_value_pair *previous_pairs = pairs;
        pairs = output_array;
        free(previous_pairs);
    }
}

int main() {
    key_value_pair *pairs = calloc(current_capacity, sizeof(key_value_pair));

    unsigned int year = 0, month = 0, day = 0;
    string value;

    while (scanf("%d.%d.%d ", &day, &month, &year) != EOF) {
        if (pair_count + 1 > current_capacity) {
            current_capacity *= 2;
            void *new_pairs_ptr = realloc(pairs, current_capacity * sizeof(key_value_pair));
            if (new_pairs_ptr != NULL) pairs = (key_value_pair*) new_pairs_ptr;
            else exit(1);
        }

        string_read(&value, stdin);
        // printf("READ: %d.%d.%d\t%s\n", day, month, year, value.values);

        pairs[pair_count].key = year * 10000 + month * 100 + day;
        pairs[pair_count].value = value;
        ++pair_count;
    }

    radix_sort(pairs);

    for (uint64_t i = 0; i < pair_count; ++i) {
        printf("%"PRIu64": ", i);
        printf("%d\t%s\n", pairs[i].key, pairs[i].value.values);
    }

    free(pairs);

    return 0;
}