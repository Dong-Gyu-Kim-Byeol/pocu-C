#ifndef DOCUMENT_ANALYZER_H
#define DOCUMENT_ANALYZER_H

#define TRUE (1)
#define FALSE (0)

/* required */
int load_document(const char* document);

void dispose(void);

size_t get_total_word_count(void);

size_t get_total_sentence_count(void);

size_t get_total_paragraph_count(void);

const char*** get_paragraph_or_null(const size_t paragraph_index);

size_t get_paragraph_word_count(const char*** paragraph);

size_t get_paragraph_sentence_count(const char*** paragraph);

const char** get_sentence_or_null(const size_t paragraph_index, const size_t sentence_index);

size_t get_sentence_word_count(const char** sentence);

int print_as_tree(const char* filename);

/* my func */
char* get_string_at_file_malloc_or_null(const char* file_path, size_t *out_strlen);

void make_words();
void make_sentences();
void make_paragraph();

#endif /* DOCUMENT_ANALYZER_H */
