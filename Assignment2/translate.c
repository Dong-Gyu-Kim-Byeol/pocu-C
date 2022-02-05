#include <assert.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>

#include "translate.h"

#define MAX_SET_SIZE (255)
#define MAX_ARGUMENT_SIZE (512)
#define LINE_SIZE (1024)

int translate(int argc, const char** argv)
{
    char translate_set[MAX_SET_SIZE] = { 0, };

    {
        translate_flag_t translate_flag = TRANSLATE_FLAG_EMPTY;

        const char* argv_from_set = NULL;
        const char* argv_to_set = NULL;

        if (argc != 3 && argc != 4) {
            return ERROR_CODE_WRONG_ARGUMENTS_NUMBER;
        }

        if (argc == 3) {
            argv_from_set = argv[1];
            argv_to_set = argv[2];
        }

        if (argc == 4) {
            const char* p_flags = argv[1];

            argv_from_set = argv[2];
            argv_to_set = argv[3];

            if (p_flags[0] != '-') {
                return ERROR_CODE_INVALID_FLAG;
            }
            ++p_flags;

            while (*p_flags != '\0') {
                switch (*p_flags) {
                case 'i':
                    if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE) == TRUE) {
                        return ERROR_CODE_INVALID_FLAG;
                    }
                    set_translate_flag(&translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE);
                    break;

                default:
                    return ERROR_CODE_INVALID_FLAG;
                }

                ++p_flags;
            }
        }

        assert(argv_from_set != NULL);
        assert(argv_to_set != NULL);

        {
            const char* p_argv_from_set = argv_from_set;
            const char* p_argv_to_set = argv_to_set;

            size_t pre_range_from_end_index = -1;
            size_t pre_range_to_end_index = -1;

            while (*p_argv_from_set != '\0') {
                char from_c = *p_argv_from_set;
                char to_c = *p_argv_to_set;

                const size_t now_from_set_index = p_argv_from_set - argv_from_set;
                const size_t now_to_set_index = p_argv_to_set - argv_to_set;

                /* from_c */
                if (from_c == '-' && now_from_set_index > pre_range_from_end_index + 1 && *(p_argv_from_set + 1) != '\0') {
                    set_translate_flag(&translate_flag, TRANSLATE_FLAG_FROM_RANGE);
                } else if (from_c == '\\') {
                    ++p_argv_from_set;

                    switch (*p_argv_from_set) {
                    case '\\':
                        from_c = '\\';
                        break;
                    case 'a':
                        from_c = '\a';
                        break;
                    case 'b':
                        from_c = '\b';
                        break;
                    case 'f':
                        from_c = '\f';
                        break;
                    case 'n':
                        from_c = '\n';
                        break;
                    case 'r':
                        from_c = '\r';
                        break;
                    case 't':
                        from_c = '\t';
                        break;
                    case 'v':
                        from_c = '\v';
                        break;
                    case '\'':
                        from_c = '\'';
                        break;
                    case '\"':
                        from_c = '\"';
                        break;

                    default:
                        return ERROR_CODE_INVALID_FORMAT;
                    }
                }

                /* to_c */
                if (to_c == '-' && now_to_set_index > pre_range_to_end_index + 1 && *(p_argv_to_set + 1) != '\0') {
                    set_translate_flag(&translate_flag, TRANSLATE_FLAG_TO_RANGE);
                } else if (to_c == '\\') {
                    ++p_argv_to_set;

                    switch (*p_argv_to_set) {
                    case '\\':
                        to_c = '\\';
                        break;
                    case 'a':
                        to_c = '\a';
                        break;
                    case 'b':
                        to_c = '\b';
                        break;
                    case 'f':
                        to_c = '\f';
                        break;
                    case 'n':
                        to_c = '\n';
                        break;
                    case 'r':
                        to_c = '\r';
                        break;
                    case 't':
                        to_c = '\t';
                        break;
                    case 'v':
                        to_c = '\v';
                        break;
                    case '\'':
                        to_c = '\'';
                        break;
                    case '\"':
                        to_c = '\"';
                        break;

                    default:
                        return ERROR_CODE_INVALID_FORMAT;
                    }
                }

                /* setting translate_set */
                if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_FROM_RANGE) == TRUE) {
                    char range_start_from_c;
                    char range_end_from_c;

                    if (from_c == '-') {
                        ++p_argv_from_set;
                        if (p_argv_from_set - argv_from_set >= MAX_ARGUMENT_SIZE) {
                            return ERROR_CODE_ARGUMENT_TOO_LONG;
                        }

                        continue;
                    }

                    range_start_from_c = *(p_argv_from_set - 2);
                    range_end_from_c = from_c;
                    if (range_start_from_c > range_end_from_c) {
                        return ERROR_CODE_INVALID_RANGE;
                    }

                    from_c = range_start_from_c + 1;
                    while (from_c <= range_end_from_c) {
                        to_c = *p_argv_to_set;

                        if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE) == TRUE) {
                            translate_set[tolower(from_c)] = to_c;
                            translate_set[toupper(from_c)] = to_c;
                        } else {
                            translate_set[(int)from_c] = to_c;
                        }


                        ++p_argv_to_set;
                        if (*p_argv_to_set == '\0') {
                            --p_argv_to_set;
                        }

                        ++from_c;
                    }


                    pre_range_from_end_index = p_argv_from_set - argv_from_set;
                    unset_translate_flag(&translate_flag, TRANSLATE_FLAG_FROM_RANGE);

                    ++p_argv_from_set;
                    if (p_argv_from_set - argv_from_set >= MAX_ARGUMENT_SIZE) {
                        return ERROR_CODE_ARGUMENT_TOO_LONG;
                    }

                    continue;

                } else if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_TO_RANGE) == TRUE) {
                    char range_start_to_c;
                    char range_end_to_c;

                    if (to_c == '-') {
                        ++p_argv_to_set;
                        if (*p_argv_to_set == '\0') {
                            --p_argv_to_set;
                        }

                        continue;
                    }

                    range_start_to_c = *(p_argv_to_set - 2);
                    range_end_to_c = to_c;
                    if (range_start_to_c > range_end_to_c) {
                        return ERROR_CODE_INVALID_RANGE;
                    }

                    to_c = range_start_to_c + 1;
                    while (to_c <= range_end_to_c) {
                        from_c = *p_argv_from_set;

                        if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE) == TRUE) {
                            translate_set[tolower(from_c)] = to_c;
                            translate_set[toupper(from_c)] = to_c;
                        } else {
                            translate_set[(int)from_c] = to_c;
                        }


                        ++p_argv_from_set;
                        if (p_argv_from_set - argv_from_set >= MAX_ARGUMENT_SIZE) {
                            return ERROR_CODE_ARGUMENT_TOO_LONG;
                        }

                        ++to_c;
                    }


                    pre_range_to_end_index = p_argv_to_set - argv_to_set;
                    unset_translate_flag(&translate_flag, TRANSLATE_FLAG_TO_RANGE);

                    ++p_argv_to_set;
                    if (*p_argv_to_set == '\0') {
                        --p_argv_to_set;
                    }

                    continue;

                } else if (is_set_translate_flag(translate_flag, TRANSLATE_FLAG_I_IGNORE_CASE) == TRUE) {
                    translate_set[tolower(from_c)] = to_c;
                    translate_set[toupper(from_c)] = to_c;
                } else {
                    translate_set[(int)from_c] = to_c;
                }


                ++p_argv_to_set;
                if (*p_argv_to_set == '\0') {
                    --p_argv_to_set;
                }

                ++p_argv_from_set;
                if (p_argv_from_set - argv_from_set >= MAX_ARGUMENT_SIZE) {
                    return ERROR_CODE_ARGUMENT_TOO_LONG;
                }
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
                const char to_c = translate_set[(int)*p_line];
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
