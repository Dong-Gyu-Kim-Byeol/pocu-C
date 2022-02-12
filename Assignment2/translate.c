#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>


#include "translate.h"
static error_code_t translate_combine_escape(const char* const argv_set, char* const out_combine_escape_set);
static error_code_t translate_range_extension(const char* const combine_escape_set, char* const out_range_extension_set);


#define TRANSLATE_SET_SIZE (255)
#define ARGUMENT_SET_SIZE (512)
#define LINE_SIZE (1024)


int translate(int argc, const char** argv)
{
    char translate_set[TRANSLATE_SET_SIZE] = { 0, };

    char from_set[ARGUMENT_SET_SIZE];
    char to_set[ARGUMENT_SET_SIZE];

    translate_flag_t translate_flag = TRANSLATE_FLAG_EMPTY;

    /* setting from_set, to_set */
    {
        char tmep_combine_escape_from_set[ARGUMENT_SET_SIZE];
        char tmep_combine_escape_to_set[ARGUMENT_SET_SIZE];

        const char* argv_from_set = NULL;
        const char* argv_to_set = NULL;

        /* check argument, setting flag */
        {
            if (argc != 3 && argc != 4) {
                return ERROR_CODE_WRONG_ARGUMENTS_NUMBER;
            }

            if (argc == 3) {
                argv_from_set = argv[1];
                argv_to_set = argv[2];
            }

            if (argc == 4) {
                const char* p_arg_options = argv[1];

                argv_from_set = argv[2];
                argv_to_set = argv[3];

                if (p_arg_options[0] != '-') {
                    return ERROR_CODE_INVALID_FLAG;
                }
                ++p_arg_options;

                while (*p_arg_options != '\0') {
                    switch (*p_arg_options) {
                    case 'i':
                        if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE) == TRUE) {
                            return ERROR_CODE_INVALID_FLAG;
                        }
                        set_translate_flag(&translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE);
                        break;

                    default:
                        return ERROR_CODE_INVALID_FLAG;
                    }

                    ++p_arg_options;
                }
            }
        }

        assert(argv_from_set != NULL);
        assert(argv_to_set != NULL);

        /* combine escape character */
        {
            error_code_t error = translate_combine_escape(argv_from_set, tmep_combine_escape_from_set);
            if (error != ERROR_CODE_NO_ERROR) {
                return error;
            }

            error = translate_combine_escape(argv_to_set, tmep_combine_escape_to_set);
            if (error != ERROR_CODE_NO_ERROR) {
                return error;
            }
        }

        /* range extension */
        {
            error_code_t error = translate_range_extension(tmep_combine_escape_from_set, from_set);
            if (error != ERROR_CODE_NO_ERROR) {
                return error;
            }

            error = translate_range_extension(tmep_combine_escape_to_set, to_set);
            if (error != ERROR_CODE_NO_ERROR) {
                return error;
            }
        }
    }

    /* setting translate_set */
    {
        char* p_from_set = from_set;
        char* p_to_set = to_set;

        while (*p_from_set != '\0') {
            if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE) == TRUE) {
                translate_set[toupper(*p_from_set)] = *p_to_set;
                translate_set[tolower(*p_from_set)] = *p_to_set;
            } else {
                translate_set[(int)*p_from_set] = *p_to_set;
            }

            ++p_from_set;
            ++p_to_set;
            if (*p_to_set == '\0') {
                --p_to_set;
            }
        }
    }

    {
        char line[LINE_SIZE];
        FILE* const input = stdin;
        FILE* const output = stdout;

        while (TRUE) {
            char* p_line = line;

            if (fgets(line, LINE_SIZE, input) == NULL) {
                clearerr(input);
                break;
            }

            while (*p_line != '\0') {
                char input_c = *p_line;
                char to_c = translate_set[(int)input_c];

                if (to_c != 0) {
                    *p_line = to_c;
                }

                ++p_line;
            }

            fprintf(output, "%s", line);
        }
    }

    return 0;
}


/* my functoin */
void set_translate_flag(translate_flag_t* const out_flag, const translate_flag_t set_flag)
{
    *out_flag |= set_flag;
}

void unset_translate_flag(translate_flag_t* const out_flag, const translate_flag_t unset_flag)
{
    *out_flag &= ~unset_flag;
}

int is_set_translate_flag(const translate_flag_t flag, const translate_flag_t is_set_flag)
{
    return flag & is_set_flag ? TRUE : FALSE;
}

error_code_t translate_combine_escape(const char* const argv_set, char* const out_combine_escape_set)
{
    const char* p_argv_set = argv_set;
    char* p_out_combine_escape_set = out_combine_escape_set;

    while (*p_argv_set != '\0') {
        char c;

        if (p_argv_set - argv_set > ARGUMENT_SET_SIZE - 2) {
            return ERROR_CODE_ARGUMENT_TOO_LONG;
        }

        c = *p_argv_set;

        if (c == '\\') {
            ++p_argv_set;

            switch (*p_argv_set) {
            case '\\':
                c = '\\';
                break;
            case 'a':
                c = '\a';
                break;
            case 'b':
                c = '\b';
                break;
            case 'f':
                c = '\f';
                break;
            case 'n':
                c = '\n';
                break;
            case 'r':
                c = '\r';
                break;
            case 't':
                c = '\t';
                break;
            case 'v':
                c = '\v';
                break;
            case '\'':
                c = '\'';
                break;
            case '\"':
                c = '\"';
                break;

            default:
                return ERROR_CODE_INVALID_FORMAT;
            }
        }

        *p_out_combine_escape_set = c;
        ++p_out_combine_escape_set;

        ++p_argv_set;
    }

    *p_out_combine_escape_set = '\0';

    return ERROR_CODE_NO_ERROR;
}

error_code_t translate_range_extension(const char* const combine_escape_set, char* const out_range_extension_set)
{
    char* p_out_range_extension_set = out_range_extension_set;

    const char* p_combine_escape_set = combine_escape_set;
    char c = *p_combine_escape_set;

    int pre_range_end_index = -1;
    char range_end_c;

    translate_flag_t translate_flag = TRANSLATE_FLAG_EMPTY;

    while (c != '\0') {
        if (p_out_range_extension_set - out_range_extension_set > ARGUMENT_SET_SIZE - 2) {
            return ERROR_CODE_ARGUMENT_TOO_LONG;
        }

        /* check c */
        if (c == '-' && p_combine_escape_set - combine_escape_set > pre_range_end_index + 1 && *(p_combine_escape_set + 1) != '\0') {
            char range_start_from_c;

            set_translate_flag(&translate_flag, TRANSLATE_FLAG_FROM_RANGE);
            ++p_combine_escape_set;

            range_start_from_c = *(p_combine_escape_set - 2);
            range_end_c = *p_combine_escape_set;
            if (range_start_from_c > range_end_c) {
                return ERROR_CODE_INVALID_RANGE;
            }

            if (range_start_from_c == range_end_c) {
                c = range_end_c;
                goto change_c;
            }

            c = range_start_from_c + 1;
        }


        /* setting set */
        *p_out_range_extension_set = c;
        ++p_out_range_extension_set;


    change_c:
        /* set next c */
        {
            int is_increase_p_combine_escape_set = TRUE;

            if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_FROM_RANGE) == TRUE) {
                if (c < range_end_c) {
                    ++c;
                    is_increase_p_combine_escape_set = FALSE;
                } else {
                    pre_range_end_index = p_combine_escape_set - combine_escape_set;
                    unset_translate_flag(&translate_flag, TRANSLATE_FLAG_FROM_RANGE);
                }
            }

            if (is_increase_p_combine_escape_set == TRUE) {
                ++p_combine_escape_set;
                c = *p_combine_escape_set;
            }
        }
    }

    *p_out_range_extension_set = '\0';

    return ERROR_CODE_NO_ERROR;
}
