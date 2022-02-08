#ifndef MY_STRING_H
#define MY_STRING_H

size_t str_length(const char* const str);

int str_to_int(const char* const str);
unsigned int str_to_uint(const char* const str);

void int_to_str(char* const out_str, const size_t buf_size, int num);
void uint_to_str(char* const out_str, const size_t buf_size, unsigned int num);

void reverse_start_end(char* const str_start, char* const str_include_end);
void reverse(char* str);
void reverse_by_words(char* str);

size_t index_of_rabin_karp(const char* const str, const char* const word);
int index_of(const char* str, const char* word);

void tokenize_get_start_and_end_or_null(char* const str_or_null, const char* const delims, char** const out_token_start, char** const out_token_include_end);
char* tokenize_or_null(char* str_or_null, const char* delims);
char* reverse_tokenize_or_null(char* str_or_null, const char* delims);

#endif /* MY_STRING_H */
