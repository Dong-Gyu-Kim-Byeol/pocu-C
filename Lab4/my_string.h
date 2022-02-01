#ifndef MY_STRING_H
#define MY_STRING_H

/* require */
void reverse(char* str);

int index_of(const char* str, const char* word);

void reverse_by_words(char* str);

char* tokenize(char* str_or_null, const char* delims);

char* reverse_tokenize(char* str_or_null, const char* delims);


/* my function */
size_t str_length(const char* const str);

size_t index_of_rabin_karp(const char* const str, const char* const word);

void reverse_start_end(char* const str_start, char* const str_include_end);

void tokenize_get_start_and_end_or_null(char* const str_or_null, const char* const delims, char** const out_token_start, char** const out_token_include_end);

#endif /* MY_STRING_H */
