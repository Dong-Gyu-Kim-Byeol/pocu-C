#ifndef TRANSLATE_H
#define TRANSLATE_H

#define TRUE (1)
#define FALSE (0)

/* required */
typedef enum error_code {
    ERROR_CODE_WRONG_ARGUMENTS_NUMBER = 1,
    ERROR_CODE_INVALID_FLAG,
    ERROR_CODE_INVALID_FORMAT,
    ERROR_CODE_ARGUMENT_TOO_LONG,
    ERROR_CODE_INVALID_RANGE
} error_code_t;

int translate(int argc, const char** argv);


/* my functoin */
typedef enum translate_flag {
    TRANSLATE_FLAG_EMPTY = 0,
    TRANSLATE_FLAG_RANGE = 1 << 0,
    TRANSLATE_FLAG_I_IGNORE_CASE = 1 << 1
} translate_flag_t;

void set_translate_flag(translate_flag_t* const out_flag, const translate_flag_t set_flag);
void unset_translate_flag(translate_flag_t* const out_flag, const translate_flag_t unset_flag);
int is_set_translate_flag(const translate_flag_t flag, const translate_flag_t is_set_flag);

#endif /* TRANSLATE_H */
