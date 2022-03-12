#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "document_analyzer.h"


#define INIT_ARRAY_CAPACITY (8)
#define INCREASE_ARRAY_SIZE (2)

#define LINE_SIZE (2)


static char* s_pa_str = NULL;
static size_t s_strlen = 0;

static char** s_pa_words = NULL;
static size_t s_word_capacity = 0;
static size_t s_word_total_count = 0;

static char*** s_pa_sentences = NULL;
static size_t s_sentence_capacity = 0;
static size_t s_sentence_total_count = 0;

static char**** s_pa_paragraphs = NULL;
static size_t s_paragraph_capacity = 0;
static size_t s_paragraph_total_count = 0;

/* static char***** s_pa_documents = NULL; */

int load_document(const char* document)
{
    dispose();
    s_pa_str = get_string_at_file_malloc_or_null(document, &s_strlen);
    if (s_pa_str == NULL) {
        dispose();
        return FALSE;
    }

    make_words();
    make_sentences();
    make_paragraph();

    return TRUE;
}

void dispose(void)
{
    free(s_pa_str);
    s_pa_str = NULL;
    s_strlen = 0;

    free(s_pa_words);
    s_pa_words = NULL;
    s_word_capacity = 0;
    s_word_total_count = 0;

    free(s_pa_sentences);
    s_pa_sentences = NULL;
    s_sentence_capacity = 0;
    s_sentence_total_count = 0;

    free(s_pa_paragraphs);
    s_pa_paragraphs = NULL;
    s_paragraph_capacity = 0;
    s_paragraph_total_count = 0;
}

size_t get_total_word_count(void)
{
    return s_word_total_count;
}

size_t get_total_sentence_count(void)
{
    return s_sentence_total_count;
}

size_t get_total_paragraph_count(void)
{
    return s_paragraph_total_count;
}

const char*** get_paragraph_or_null(const size_t paragraph_index)
{
    const size_t max_i = s_paragraph_total_count - 1;
    if (s_paragraph_total_count == 0 || paragraph_index > max_i) {
        return NULL;
    }

    return (const char***)s_pa_paragraphs[paragraph_index];
}

size_t get_paragraph_word_count(const char*** paragraph)
{
    size_t word_count = 0;
    const char*** p_sentences = paragraph;
    while (*p_sentences != NULL) {
        word_count += get_sentence_word_count(*p_sentences);
        ++p_sentences;
    }

    return word_count;
}

size_t get_paragraph_sentence_count(const char*** paragraph)
{
    size_t count = 0;
    const char*** p_sentences = paragraph;
    while (*p_sentences != NULL) {
        ++count;
        ++p_sentences;
    }

    return count;
}

const char** get_sentence_or_null(const size_t paragraph_index, const size_t sentence_index)
{
    size_t i = 0;
    const char*** p_sentences = get_paragraph_or_null(paragraph_index);
    if (p_sentences == NULL || s_sentence_total_count == 0) {
        return NULL;
    }

    while (*p_sentences != NULL) {
        if (sentence_index == i) {
            return (const char**)(*p_sentences);
        }

        ++i;
        ++p_sentences;
    }

    return NULL;
}

size_t get_sentence_word_count(const char** sentence)
{
    size_t word_count = 0;
    const char** p_words = sentence;
    while (*p_words != NULL) {
        ++word_count;
        ++p_words;
    }

    return word_count;
}

int print_as_tree(const char* filename)
{
    size_t pi;
    FILE* file = NULL;
    if (s_pa_str = NULL) {
        return FALSE;
    }

    file = fopen(filename, "w");
    if (file == NULL) {
        return FALSE;
    }

    for (pi = 0; pi < s_paragraph_total_count; ++pi) {
        const char*** p_sentences = get_paragraph_or_null(pi);
        size_t si = 0;

        if (pi != 0) {
            fprintf(file, "\n");
        }

        fprintf(file, "Paragraph %lu:", pi);

        while (*p_sentences != NULL) {
            const char** p_words = *p_sentences;

            fprintf(file, "\n    Sentence %lu:", si);

            while (*p_words != NULL) {
                fprintf(file, "\n        %s", *p_words);
                ++p_words;
            }

            ++p_sentences;
            ++si;
        }

        if (pi < s_paragraph_total_count - 1) {
            fprintf(file, "\n");
        }
    }

    fclose(file);
    file = NULL;
    return TRUE;
}


/* my func */
char* get_string_at_file_malloc_or_null(const char* file_path, size_t* out_strlen)
{
    char line[LINE_SIZE];

    char* pa_str = NULL;
    size_t str_capacity = LINE_SIZE;
    size_t strlen = 0;

    char* p_str = NULL;

    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        return NULL;
    }

    pa_str = malloc(sizeof(*pa_str) * LINE_SIZE);
    p_str = pa_str;

    while (fgets(line, LINE_SIZE, file) != NULL) {
        char* p_line = line;
        while (*p_line != '\0') {
            const size_t next_i = p_str - pa_str;
            if (next_i >= str_capacity - 1) {
                char* pa_tmp = NULL;
                const size_t new_capacity = str_capacity * INCREASE_ARRAY_SIZE;

                pa_tmp = malloc(sizeof(*pa_str) * new_capacity);
                memcpy(pa_tmp, pa_str, str_capacity);

                free(pa_str);
                pa_str = NULL;

                pa_str = pa_tmp;
                str_capacity = new_capacity;
                p_str = pa_str + next_i;
            }

            *p_str = *p_line;

            ++p_str;
            ++strlen;

            ++p_line;
        }
    }

    fclose(file);
    file = NULL;

    *p_str = '\0';
    *out_strlen = strlen;
    return pa_str;
}

void make_words()
{
    int is_add_new_null = FALSE;
    int is_add_new_word = TRUE;

    char** p_words = NULL;
    char* p_str = s_pa_str;

    while (*p_str != '\0') {
        switch (*p_str) {
        case '\n':
            is_add_new_null = TRUE;
            is_add_new_word = TRUE;
            break;

        case '.':
            /* intentional fallthrough */
        case '!':
            /* intentional fallthrough */
        case '?':
            is_add_new_null = TRUE;
            is_add_new_word = TRUE;
            break;

        case ' ':
            /* intentional fallthrough */
        case ',':
            is_add_new_word = TRUE;
            *p_str = '\0';
            break;

        default:
            /* add word */
            if (is_add_new_word) {
                if (s_pa_words == NULL) {
                    s_pa_words = malloc(sizeof(*s_pa_words) * INIT_ARRAY_CAPACITY);
                    s_word_capacity = INIT_ARRAY_CAPACITY;
                    s_word_total_count = 0;

                    p_words = s_pa_words;
                    *p_words = NULL;
                }

                if ((size_t)(p_words - s_pa_words) >= s_word_capacity - 2) {
                    const size_t i = p_words - s_pa_words;
                    const size_t new_capacity = s_word_capacity * INCREASE_ARRAY_SIZE;
                    char** pa_tmp = malloc(sizeof(*s_pa_words) * new_capacity);
                    memcpy(pa_tmp, s_pa_words, sizeof(*s_pa_words) * s_word_capacity);

                    free(s_pa_words);
                    s_pa_words = NULL;

                    s_pa_words = pa_tmp;
                    s_word_capacity = new_capacity;

                    p_words = s_pa_words + i;
                }

                assert(*p_words == NULL);
                if (p_words != s_pa_words && is_add_new_null == TRUE) {
                    ++p_words;
                }
                *p_words = p_str;

                ++s_word_total_count;
                ++p_words;
                *p_words = NULL;

                is_add_new_word = FALSE;
                is_add_new_null = FALSE;
            }
            break;
        }

        ++p_str;
    }
}

void make_sentences()
{
    int is_add_new_null = FALSE;
    int is_add_new_sentence = TRUE;

    char*** p_sentences = NULL;
    char** p_words = s_pa_words;
    char* p_str = s_pa_str;

    size_t i = s_strlen;
    while (i != 0) {
        switch (*p_str) {
        case '\n':
            is_add_new_null = TRUE;
            is_add_new_sentence = TRUE;
            break;

        case '.':
            /* intentional fallthrough */
        case '!':
            /* intentional fallthrough */
        case '?':
            is_add_new_sentence = TRUE;
            *p_str = '\0';
            break;

        case '\0':
            break;

        default:
            /* add sentence */
            if (is_add_new_sentence) {
                if (s_pa_sentences == NULL) {
                    s_pa_sentences = malloc(sizeof(*s_pa_sentences) * INIT_ARRAY_CAPACITY);
                    s_sentence_capacity = INIT_ARRAY_CAPACITY;
                    s_sentence_total_count = 0;

                    p_sentences = s_pa_sentences;
                    *p_sentences = NULL;
                }

                if ((size_t)(p_sentences - s_pa_sentences) >= s_sentence_capacity - 2) {
                    const size_t i = p_sentences - s_pa_sentences;
                    const size_t new_capacity = s_sentence_capacity * INCREASE_ARRAY_SIZE;
                    char*** pa_tmp = malloc(sizeof(*s_pa_sentences) * new_capacity);
                    memcpy(pa_tmp, s_pa_sentences, sizeof(*s_pa_sentences) * s_sentence_capacity);

                    free(s_pa_sentences);
                    s_pa_sentences = NULL;

                    s_pa_sentences = pa_tmp;
                    s_sentence_capacity = new_capacity;

                    p_sentences = s_pa_sentences + i;
                }

                assert(*p_sentences == NULL);
                if (p_sentences != s_pa_sentences && is_add_new_null == TRUE) {
                    ++p_sentences;
                }
                *p_sentences = p_words;

                while (*p_words != NULL) {
                    ++p_words;
                }
                ++p_words;

                ++s_sentence_total_count;
                ++p_sentences;
                *p_sentences = NULL;

                is_add_new_null = FALSE;
                is_add_new_sentence = FALSE;
            }
            break;
        }

        ++p_str;
        --i;
    }
}

void make_paragraph()
{
    int is_add_new_paragraph = TRUE;

    char**** p_paragraphs = NULL;
    char*** p_sentences = s_pa_sentences;
    char* p_str = s_pa_str;

    size_t i = s_strlen;
    while (i != 0) {
        switch (*p_str) {
        case '\n':
            is_add_new_paragraph = TRUE;
            break;

        case '\0':
            break;

        default:
            /* add paragraph */
            if (is_add_new_paragraph) {
                if (s_pa_paragraphs == NULL) {
                    s_pa_paragraphs = malloc(sizeof(*s_pa_paragraphs) * INIT_ARRAY_CAPACITY);
                    s_paragraph_capacity = INIT_ARRAY_CAPACITY;
                    s_paragraph_total_count = 0;

                    p_paragraphs = s_pa_paragraphs;
                    *p_paragraphs = NULL;
                }

                if ((size_t)(p_paragraphs - s_pa_paragraphs) >= s_paragraph_capacity - 1) {
                    const size_t i = p_paragraphs - s_pa_paragraphs;
                    const size_t new_capacity = s_paragraph_capacity * INCREASE_ARRAY_SIZE;
                    char**** pa_tmp = malloc(sizeof(*s_pa_paragraphs) * new_capacity);
                    memcpy(pa_tmp, s_pa_paragraphs, sizeof(*s_pa_paragraphs) * s_paragraph_capacity);

                    free(s_pa_paragraphs);
                    s_pa_paragraphs = NULL;

                    s_pa_paragraphs = pa_tmp;
                    s_paragraph_capacity = new_capacity;

                    p_paragraphs = s_pa_paragraphs + i;
                }

                assert(*p_paragraphs == NULL);
                *p_paragraphs = p_sentences;

                while (*p_sentences != NULL) {
                    ++p_sentences;
                }
                ++p_sentences;

                ++s_paragraph_total_count;
                ++p_paragraphs;
                *p_paragraphs = NULL;

                is_add_new_paragraph = FALSE;
            }
            break;
        }

        ++p_str;
        --i;
    }
}
