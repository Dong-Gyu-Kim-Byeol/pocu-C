#include "tokenize.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define TOKENS_CAPACITY_INCREASE (2)
#define TRUE (1)
#define FALSE (0)

char** tokenize_malloc(const char* str, const char* delims)
{
    const char* p_str = NULL;

    size_t tokens_capacity = TOKENS_CAPACITY_INCREASE;
    char** pa_tokens = NULL;
    char** tokens = NULL;

    pa_tokens = malloc(sizeof(*tokens) * tokens_capacity);
    tokens = pa_tokens;
    *tokens = NULL;

    assert(str != NULL);
    assert(delims != NULL);

    if (*delims == '\0') {
        const size_t size = strlen(str) + 1;
        char* pa_token = malloc(size);
        memcpy(pa_token, str, size);

        assert(tokens_capacity >= 2);
        *tokens = pa_token;
        tokens++;
        *tokens = NULL;

        return pa_tokens;
    }

    p_str = str;
    while (TRUE) {
        /* skip delims */
        while (*p_str != '\0') {
            const char* p_delims = delims;

            while (*p_delims != '\0') {
                if (*p_str == *p_delims) {
                    break;
                }

                ++p_delims;
            }

            /* "*p_str" is not delim character */
            if (*p_delims == '\0') {
                break;
            }

            ++p_str;
        }

        /* make token */
        {
            const char* const p_str_first = p_str;
            while (*p_str_first != '\0') {
                const char* p_delims = delims;

                while (*p_delims != '\0') {
                    if (*p_str != *p_delims && *p_str != '\0') {
                        ++p_delims;
                        continue;
                    }

                    /* make token */
                    /* "*p_str" is delim character or '\0' character */
                    {
                        char* pa_token = malloc(p_str - p_str_first + 1);
                        memcpy(pa_token, p_str_first, p_str - p_str_first);
                        pa_token[p_str - p_str_first] = '\0';

                        if ((size_t)(tokens - pa_tokens) == tokens_capacity - 1) {
                            char** pa_tmp = NULL;
                            const size_t size = tokens - pa_tokens;
                            tokens_capacity *= TOKENS_CAPACITY_INCREASE;

                            pa_tmp = malloc(sizeof(pa_token) * tokens_capacity);
                            memcpy(pa_tmp, pa_tokens, sizeof(pa_token) * size);

                            free(pa_tokens);
                            pa_tokens = NULL;

                            pa_tokens = pa_tmp;
                            tokens = pa_tokens + size;
                        }

                        *tokens = pa_token;
                        tokens++;
                        *tokens = NULL;

                        goto made_token;
                    }
                }

                ++p_str;
            }
        }

    made_token:
        if (*p_str == '\0') {
            return pa_tokens;
        }

        ++p_str;
    }

    assert(FALSE);
    free(pa_tokens);
    pa_tokens = NULL;
    return NULL;
}
