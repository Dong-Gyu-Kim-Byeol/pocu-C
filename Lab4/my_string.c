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
    return (int)index_of_rabin_karp(str, word);
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

char* tokenize(char* str_or_null, const char* delims)
{
    char* p_token_start;
    char* p_token_include_end;

    assert(delims != NULL);

    tokenize_get_start_and_end_or_null(str_or_null, delims, &p_token_start, &p_token_include_end);

    return p_token_start;
}

char* reverse_tokenize(char* str_or_null, const char* delims)
{
    char* p_token_start;
    char* p_token_end;

    assert(delims != NULL);

    tokenize_get_start_and_end_or_null(str_or_null, delims, &p_token_start, &p_token_end);

    if (p_token_start != NULL) {
        assert(p_token_end != NULL);
        reverse_start_end(p_token_start, p_token_end);
    }

    return p_token_start;
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

    /* skip */
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
        s_str = NULL;
        return;
    }
}
