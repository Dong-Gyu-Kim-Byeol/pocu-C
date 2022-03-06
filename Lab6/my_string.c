#include <assert.h>

#include "my_string.h"

#define TRUE (1)
#define FALSE (0)

size_t str_length(const char* const str)
{
    const char* p_str = str;

    assert(str != NULL);

    while (*p_str != '\0') {
        ++p_str;
    }

    return p_str - str;
}

int str_to_int(const char* const str)
{
    int num = 0;
    const char* p_str = str;

    while (*p_str != '\0') {
        const char num_c = *p_str;

        num *= 10;
        num += num_c - '0';

        ++p_str;
    }

    return num;
}

unsigned int str_to_uint(const char* const str)
{
    unsigned int num = 0;
    const char* p_str = str;

    while (*p_str != '\0') {
        const char num_c = *p_str;

        num *= 10;
        num += num_c - '0';

        ++p_str;
    }

    return num;
}

void int_to_str(char* const out_str, const size_t buf_size, int num)
{
    char* p_out_str_start = out_str;
    char* p_out_str = out_str;
    assert(buf_size > 11);

    p_out_str_start[11] = '\0';
    if (num < 0) {
        *p_out_str_start = '-';
        ++p_out_str_start;
    }

    while (TRUE) {
        *p_out_str = (num % 10) + '0';
        num /= 10;

        ++p_out_str;
        if (num == 0) {
            break;
        }
    }

    *p_out_str = '\0';
    reverse_start_end(p_out_str_start, p_out_str - 1);
}

void uint_to_str(char* const out_str, const size_t buf_size, unsigned int num)
{
    char* p_out_str = out_str;
    assert(buf_size > 10);

    p_out_str[10] = '\0';

    while (TRUE) {
        *p_out_str = (num % 10) + '0';
        num /= 10;

        ++p_out_str;
        if (num == 0) {
            break;
        }
    }

    *p_out_str = '\0';
    reverse_start_end(out_str, p_out_str - 1);
}

void reverse_start_end(char* const str_start, char* const str_include_end)
{
    char* p_front = str_start;
    char* p_back = str_include_end;

    assert(str_start != NULL);
    assert(str_include_end != NULL);
    assert(*str_include_end != '\0');
    assert(str_start <= str_include_end);

    while (p_front < p_back) {
        char temp = *p_back;
        *p_back = *p_front;
        *p_front = temp;

        ++p_front;
        --p_back;
    }
}

void reverse(char* str)
{
    assert(str != NULL);

    if (*str == '\0') {
        return;
    }

    {
        char* const p_str = str;
        char* const p_back = str + (str_length(str) - 1);

        reverse_start_end(p_str, p_back);
    }
}

void reverse_by_words(char* str)
{
    char* p_str_first = str;

    assert(str != NULL);

    while (*p_str_first != '\0') {
        char* p_str;

        /* skip */
        while (*p_str_first == ' ') {
            ++p_str_first;
        }

        p_str = p_str_first;

        while (*p_str != ' ' && *p_str != '\0') {
            ++p_str;
        }

        assert(*p_str == ' ' || *p_str == '\0');
        assert(p_str_first <= p_str - 1);
        reverse_start_end(p_str_first, p_str - 1);

        p_str_first = p_str;
    }
}

size_t index_of_rabin_karp(const char* const str, const char* const word)
{
    const size_t MUL = 256;
    const size_t MOD_PRIME = 101;

    size_t str_hash;
    const size_t word_hash;
    const size_t last_mod_mul;

    const size_t word_length;

    assert(str != NULL);
    assert(word != NULL);

    if (*word == '\0') {
        return 0;
    }

    {
        size_t* p_word_hash = (size_t*)&word_hash;
        size_t* p_last_mod_mul = (size_t*)&last_mod_mul;

        const char* p_str = str;
        const char* p_word = word;

        str_hash = 0;
        *p_word_hash = 0;
        *p_last_mod_mul = 1;

        while (*p_word != '\0') {
            if (*p_str == '\0') {
                return (size_t)-1;
            }
            str_hash *= MUL;
            str_hash %= MOD_PRIME;
            str_hash += *p_str;
            str_hash %= MOD_PRIME;

            *p_word_hash *= MUL;
            *p_word_hash %= MOD_PRIME;
            *p_word_hash += *p_word;
            *p_word_hash %= MOD_PRIME;

            if (p_word != word) {
                *p_last_mod_mul *= MUL;
                *p_last_mod_mul %= MOD_PRIME;
            }

            ++p_str;
            ++p_word;
        }

        *(size_t*)&word_length = p_word - word;
    }

    {
        const char* p_str_first = str;

        while (1) {
            if (str_hash == word_hash) {
                const char* p_str = p_str_first;
                const char* p_word = word;

                while (*p_word != '\0' && *p_str == *p_word) {
                    ++p_str;
                    ++p_word;
                }

                if (*p_word == '\0') {
                    goto find;
                }
            }

            if (*(p_str_first + word_length) == '\0') {
                break;
            }

            str_hash += MOD_PRIME;
            str_hash -= (*p_str_first * last_mod_mul) % MOD_PRIME;
            str_hash %= MOD_PRIME;

            str_hash *= MUL;
            str_hash %= MOD_PRIME;

            str_hash += *(p_str_first + word_length);
            str_hash %= MOD_PRIME;

            ++p_str_first;
        }

        assert(*(p_str_first + word_length) == '\0');
        return (size_t)-1;

    find:
        return p_str_first - str;
    }
}

int index_of(const char* str, const char* word)
{
    return (int)index_of_rabin_karp(str, word);
}

void tokenize_get_end_or_null(
    char* const str,
    const char* const delims,
    char** const out_token_start,
    char** const out_token_include_end,
    char** const out_next_start_point)
{
    char* p_str = NULL;

    assert(out_token_start != NULL);
    assert(out_token_include_end != NULL);

    p_str = str;

    if (p_str == NULL || *p_str == '\0') {
        *out_token_start = NULL;
        *out_token_include_end = NULL;
        *out_next_start_point = NULL;
        return;
    }

    /* skip */
    while (*p_str != '\0') {
        const char* p_delims = delims;

        while (*p_delims != '\0') {
            if (*p_str == *p_delims) {
                break;
            }

            ++p_delims;
        }

        if (*p_delims == '\0') {
            break;
        }

        ++p_str;
    }

    {
        char* const p_str_first = p_str;
        ++p_str;

        while (*p_str != '\0') {
            const char* p_delims = delims;

            while (*p_delims != '\0') {
                if (*p_str == *p_delims) {
                    *p_str = '\0';
                    ++p_str;

                    *out_token_start = p_str_first;
                    *out_token_include_end = p_str - 2;
                    *out_next_start_point = p_str;
                    return;
                }

                ++p_delims;
            }

            ++p_str;
        }

        assert(*p_str == '\0');
        *out_token_start = p_str_first;
        *out_token_include_end = p_str - 1;
        *out_next_start_point = NULL;
        return;
    }
}

void tokenize_or_null(char* str, const char* delims, char** const out_token_start, char** const out_next_start_point)
{
    char* p_token_end;

    assert(delims != NULL);

    tokenize_get_end_or_null(str, delims, out_token_start, &p_token_end, out_next_start_point);

    return;
}

void reverse_tokenize_or_null(char* str, const char* delims, char** const out_token_start, char** const out_next_start_point)
{
    char* p_token_end;

    assert(delims != NULL);

    tokenize_get_end_or_null(str, delims, out_token_start, &p_token_end, out_next_start_point);

    if (*out_token_start != NULL) {
        assert(p_token_end != NULL);
        reverse_start_end(*out_token_start, p_token_end);
    }

    return;
}
