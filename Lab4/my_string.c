#include "my_string.h"
#include <assert.h>

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

int index_of(const char* str, const char* word)
{
    const char* p_str_first = str;

    assert(str != NULL);
    assert(word != NULL);

    while (*p_str_first != '\0') {
        const char* p_str = p_str_first;
        const char* p_word = word;

        while (*p_word != '\0' && *p_str == *p_word) {
            ++p_str;
            ++p_word;
        }

        if (*p_word == '\0') {
            goto find;
        }

        ++p_str_first;
    }

    assert(*p_str_first == '\0');
    return -1;

find:
    return p_str_first - str;
}

void reverse_by_words(char* str)
{
    return;
}

char* tokenize(char* str_or_null, const char* delims)
{
    return NULL;
}

char* reverse_tokenize(char* str_or_null, const char* delims)
{
    return NULL;
}


/* my function */
size_t str_length(const char* const str)
{
    const char* p_str = str;

    assert(str != NULL);

    while (*p_str != '\0') {
        ++p_str;
    }

    return p_str - str;
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

void tokenize_get_start_and_end_or_null(
    char* const str_or_null,
    const char* const delims,
    char** const out_token_start,
    char** const out_token_include_end)
{
    static char* s_str = NULL;

    assert(out_token_start != NULL);
    assert(out_token_include_end != NULL);

    if (str_or_null != NULL) {
        s_str = str_or_null;
    }

    if (s_str == NULL || *s_str == '\0') {
        *out_token_start = NULL;
        *out_token_include_end = NULL;
        return;
    }

    while (*s_str != '\0') {
        const char* p_delims = delims;

        while (*p_delims != '\0') {
            if (*s_str == *p_delims) {
                break;
            }

            ++p_delims;
        }

        if (*p_delims == '\0') {
            break;
        }

        ++s_str;
    }

    {
        char* const s_str_first = s_str;
        ++s_str;

        while (*s_str != '\0') {
            const char* p_delims = delims;

            while (*p_delims != '\0') {
                if (*s_str == *p_delims) {
                    *s_str = '\0';
                    ++s_str;

                    *out_token_start = s_str_first;
                    *out_token_include_end = s_str - 2;
                    return;
                }

                ++p_delims;
            }

            ++s_str;
        }

        assert(*s_str == '\0');
        *out_token_start = s_str_first;
        *out_token_include_end = s_str - 1;
        return;
    }
}
