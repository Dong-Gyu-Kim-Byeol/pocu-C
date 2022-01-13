#include "array.h"

#include <limits.h>

int get_index_of(const int numbers[], const size_t element_count, const int num)
{
    const int* ptr = NULL;
    const int* const end = numbers + element_count;
    for (ptr = numbers; ptr < end; ++ptr) {
        if (*ptr == num) {
            return ptr - numbers;
        }
    }

    return -1;
}

int get_last_index_of(const int numbers[], const size_t element_count, const int num)
{
    const int* ptr = NULL;
    for (ptr = numbers + element_count - 1; ptr >= numbers; --ptr) {
        if (*ptr == num) {
            return ptr - numbers;
        }
    }

    return -1;
}

int get_max_index(const int numbers[], const size_t element_count)
{
    const int* max = numbers;
    const int* ptr = NULL;
    const int* const end = numbers + element_count;

    if (element_count == 0) {
        return -1;
    }

    for (ptr = numbers; ptr < end; ++ptr) {
        if (*max < *ptr) {
            max = ptr;
        }
    }

    return max - numbers;
}

int get_min_index(const int numbers[], const size_t element_count)
{
    const int* min = numbers;
    const int* ptr = NULL;
    const int* const end = numbers + element_count;

    if (element_count == 0) {
        return -1;
    }

    for (ptr = numbers; ptr < end; ++ptr) {
        if (*min > *ptr) {
            min = ptr;
        }
    }

    return min - numbers;
}

int is_all_positive(const int numbers[], const size_t element_count)
{
    const int* ptr = NULL;
    const int* const end = numbers + element_count;

    if (element_count == 0) {
        return FALSE;
    }

    for (ptr = numbers; ptr < end; ++ptr) {
        if (*ptr < 0) {
            return FALSE;
        }
    }

    return TRUE;
}

int has_even(const int numbers[], const size_t element_count)
{
    const int* ptr = NULL;
    const int* const end = numbers + element_count;

    if (element_count == 0) {
        return FALSE;
    }

    for (ptr = numbers; ptr < end; ++ptr) {
        if (*ptr % 2 == 0) {
            return TRUE;
        }
    }

    return FALSE;
}

int insert(int numbers[], const size_t element_count, const int num, const size_t pos)
{
    int* ptr = NULL;
    const int* const begin = numbers + pos;

    if (pos > element_count) {
        return FALSE;
    }

    for (ptr = numbers + element_count - 1; ptr >= begin; --ptr) {
        *(ptr + 1) = *ptr;
    }
    numbers[pos] = num;

    return TRUE;
}

int remove_at(int numbers[], const size_t element_count, const size_t index)
{
    int* ptr = NULL;
    const int* const end = numbers + element_count;

    if (index >= element_count) {
        return FALSE;
    }

    for (ptr = numbers + index; ptr < end; ++ptr) {
        *ptr = *(ptr + 1);
    }
    numbers[element_count - 1] = INT_MIN;

    return TRUE;
}
