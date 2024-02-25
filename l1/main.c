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
#include <math.h>

#include <inttypes.h>
#include <stdio.h>

typedef struct {
    char *values;
    uint32_t last_element;
    uint32_t memory_size;
} string;


#include <stdlib.h>
#include <string.h>

//? Maybe deprecated, but those are for error handling
#define UNDEFINED 0
#define SUCCESS 1
#define ERROR -1

// How big initial memory is
const uint32_t INIT_CAPACITY = 2;
// How big relative to previous allocated memory new memory should be
const uint32_t EXTENDED_CAPACITY = 2;

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

void string_free(string s) {
    if (s.values != NULL) free(s.values);
}

int string_init(string* s) {
    int result = UNDEFINED;

    s->memory_size = INIT_CAPACITY;
    s->values = (char*) calloc(INIT_CAPACITY, sizeof(char));
    s->last_element = 0;
    s->values[0] = '\0';

    return result;
}

int string_resize(string* s, uint32_t capacity) {
    s->memory_size *= capacity;
    s->values = (char*) realloc(s->values, sizeof(char) * s->memory_size);

    return SUCCESS;
}

int string_push_char(string* s, char value) {
    int result = UNDEFINED;

    if (s->last_element < s->memory_size - 1) {
        result = SUCCESS;
    } else {
        result = string_resize(s, EXTENDED_CAPACITY);
    }

    s->values[s->last_element] = value;
    s->values[s->last_element + 1] = '\0';
    s->last_element++;

    return result;
}

int string_push_str(string* s, char values[]) {
    int result = UNDEFINED;

    uint32_t l = strlen(values);
    for (uint32_t i = 0; i < l; ++i) {
        result = string_push_char(s, values[i]);
        if (result != SUCCESS) return result;
    }

    return result;
}

int string_push_string(string* s1, string s2) {
    int result = UNDEFINED;

    result = string_push_str(s1, s2.values);

    return result;
}

int string_copy(string* s1, string s2) {
    int result = UNDEFINED;
    uint32_t minimal = min(s1->last_element, s2.last_element);
    for (uint32_t i = 0; i < s1->last_element; ++i) {
        if (i < minimal) {
            s1->values[i] = s2.values[i];
        } else {
            s1->values[i] = ' ';
        }
    }
    s1->last_element = minimal;
    return result;
}

int string_set(string* s1, string s2) {
    int result = UNDEFINED;

    string_free(*s1);
    result = string_init(s1);
    result = string_push_string(s1, s2);

    return result;
}

int string_read(string* s, FILE* stream) {
    string_free(*s);
    string_init(s);
    int c = 0;
    int end = 0;

    while (1) {
        c = fgetc(stream);
        char ch = (char) c;

        if (c == EOF) {
            end = EOF;
            break;
        } else if (ch == '\t' || ch == '\n') {
            break;
        }

        string_push_char(s, ch);
    }

    return end;
}

int64_t string_index(string s, char c) {
    for (int64_t i = 0; i < s.last_element; ++i) {
        if (s.values[i] == c) {
            return i;
        }
    }

    return ERROR;
}

int string_contains(string s, char c) {
    for (uint32_t i = 0; i < s.last_element; ++i) {
        if (s.values[i] == c) {
            return 1;
        }
    }

    return 0;
}

uint32_t string_len(string s) {
    return s.last_element;
}

int string_at(string s, uint32_t i) {
    if (i <= s.last_element) {
        return s.values[i];
    }

    return ERROR;
}


const int initial_byte_mask = 0xFF;

uint64_t pair_count = 0;
uint64_t current_capacity = 2;


typedef struct {
    int key; // key = year * 10000 + month * 100 + day, max key is 99991231 ()
    string input_key;
    string value;
} key_value_pair;


void radix_sort(key_value_pair **pairs) {
    for (uint64_t byte_shift = 0; byte_shift < sizeof(unsigned); ++byte_shift) {
        uint64_t *sorting_array = calloc(initial_byte_mask + 1, sizeof(uint64_t));
        uint64_t binary_shift = byte_shift * ceil(log2(initial_byte_mask));
        key_value_pair *output_array = calloc(current_capacity, sizeof(key_value_pair));

        unsigned byte_mask = initial_byte_mask << binary_shift;

        // printf("%"PRIu64" bin shift => byte_mask is 0x%x\n", binary_shift, byte_mask);

        for (uint64_t pair_index = 0; pair_index < pair_count; ++pair_index) {
            int byte = ((*pairs)[pair_index].key & byte_mask) >> binary_shift;
            ++sorting_array[byte];
            // if ((*pairs)[pair_index].input_key.values == NULL)
            // printf("+ %"PRIu64", sorting_array[%d] = %"PRIu64"\n", pair_index, byte, sorting_array[byte]);
        }

        for (int byte = 1; byte <= initial_byte_mask; ++byte) sorting_array[byte] += sorting_array[byte - 1];

        for (uint64_t pair_index = pair_count; pair_index > 0; --pair_index) {
            int byte = ((*pairs)[pair_index - 1].key & byte_mask) >> binary_shift;
            --sorting_array[byte];
            output_array[sorting_array[byte]] = (*pairs)[pair_index - 1];

            // if ((*pairs)[pair_index - 1].input_key.values == NULL)
            // printf("- %"PRIu64", sorting_array[%d] = %"PRIu64", pairs[%"PRIu64"].input_key = %s\n",
            //     pair_index - 1, byte, sorting_array[byte], pair_index - 1, (*pairs)[pair_index - 1].input_key.values
            // );
            
            // if (output_array[sorting_array[byte]].input_key.values == NULL) 
            //     printf("YOU FUCKING NULLED!!!\n");
        }

        // for (uint64_t i = 0; i < pair_count; ++i) {
        //     if (output_array[i].input_key.values == NULL)
        //     printf("i = %"PRIu64"\n", i);
        // }

        key_value_pair *previous_pairs = *pairs;
        *pairs = output_array;

        free(previous_pairs);
        free(sorting_array);
    }
}

int main() {
    key_value_pair *pairs = calloc(current_capacity, sizeof(key_value_pair));

    unsigned int year = 0, month = 0, day = 0;

    string value;
    string input_key;

    string_init(&value);
    string_init(&input_key);

    while (string_read(&input_key, stdin) != EOF) {
        if (pair_count + 1 > current_capacity) {
            current_capacity *= 2;
            void *new_pairs_ptr = realloc(pairs, current_capacity * sizeof(key_value_pair));
            if (new_pairs_ptr != NULL) pairs = (key_value_pair*) new_pairs_ptr;
            else exit(1);
        }

        sscanf(input_key.values, "%d.%d.%d ", &day, &month, &year);
        string_read(&value, stdin);

        pairs[pair_count].key = year * 10000 + month * 100 + day;
        string_init(&pairs[pair_count].input_key);
        string_set(&pairs[pair_count].input_key, input_key);
        string_init(&pairs[pair_count].value);
        string_set(&pairs[pair_count].value, value);

        // printf("READ: %d.%d.%d\t%s\n", day, month, year, value.values);
        // printf("input_key: %s, key: %d\n",
        //     pairs[pair_count].input_key.values,
        //     pairs[pair_count].key,
        //     pairs[pair_count].value.values
        // );
        ++pair_count;

        if (feof(stdin)) break;
    }

    string_free(value);
    string_free(input_key);

    // printf("COUNT: %"PRIu64"\n", pair_count);

    radix_sort(&pairs);

    for (uint64_t i = 0; i < pair_count; ++i) {
        printf("%s\t%s\n", pairs[i].input_key.values, pairs[i].value.values);
        string_free(pairs[i].input_key);
        string_free(pairs[i].value);
    }

    free(pairs);

    return 0;
}