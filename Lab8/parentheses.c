#include <stdlib.h>

#include "parentheses.h"

typedef struct {
    const char* opening_parentheses;
} parentheses_stack_t;

int compare_parenthesis(const void* a, const void* b)
{
    const parenthesis_t* const p1 = a;
    const parenthesis_t* const p2 = b;

    return p1->opening_index - p2->opening_index;
}

size_t get_matching_parentheses(parenthesis_t* out_parentheses, size_t max_size, const char* str)
{
    size_t out_parentheses_count = 0;

    parentheses_stack_t* pa_parentheses_stacks = malloc(sizeof(parentheses_stack_t) * max_size * 4);

    parentheses_stack_t* round_parentheses_stack = pa_parentheses_stacks + max_size * 0;
    size_t round_parentheses_sp = (size_t)-1;

    parentheses_stack_t* curly_parentheses_stack = pa_parentheses_stacks + max_size * 1;
    size_t curly_parentheses_sp = (size_t)-1;

    parentheses_stack_t* square_parentheses_stack = pa_parentheses_stacks + max_size * 2;
    size_t square_parentheses_sp = (size_t)-1;

    parentheses_stack_t* inequality_parentheses_stack = pa_parentheses_stacks + max_size * 3;
    size_t inequality_parentheses_sp = (size_t)-1;

    const char* p_str = str;

    while (*p_str != '\0') {
        if (out_parentheses_count == max_size) {
            break;
        }

        switch (*p_str) {
        case '(':
            ++round_parentheses_sp;
            round_parentheses_stack[round_parentheses_sp].opening_parentheses = p_str;
            break;
        case '{':
            ++curly_parentheses_sp;
            curly_parentheses_stack[curly_parentheses_sp].opening_parentheses = p_str;
            break;
        case '[':
            ++square_parentheses_sp;
            square_parentheses_stack[square_parentheses_sp].opening_parentheses = p_str;
            break;
        case '<':
            ++inequality_parentheses_sp;
            inequality_parentheses_stack[inequality_parentheses_sp].opening_parentheses = p_str;
            break;

        case ')':
            if (round_parentheses_sp != (size_t)-1) {
                size_t i = out_parentheses_count;
                out_parentheses[i].opening_index = round_parentheses_stack[round_parentheses_sp].opening_parentheses - str;
                out_parentheses[i].closing_index = p_str - str;
                --round_parentheses_sp;

                ++out_parentheses_count;
            }
            break;
        case '}':
            if (curly_parentheses_sp != (size_t)-1) {
                size_t i = out_parentheses_count;
                out_parentheses[i].opening_index = curly_parentheses_stack[curly_parentheses_sp].opening_parentheses - str;
                out_parentheses[i].closing_index = p_str - str;
                --curly_parentheses_sp;

                ++out_parentheses_count;
            }
            break;
        case ']':
            if (square_parentheses_sp != (size_t)-1) {
                size_t i = out_parentheses_count;
                out_parentheses[i].opening_index = square_parentheses_stack[square_parentheses_sp].opening_parentheses - str;
                out_parentheses[i].closing_index = p_str - str;
                --square_parentheses_sp;

                ++out_parentheses_count;
            }
            break;
        case '>':
            if (inequality_parentheses_sp != (size_t)-1) {
                size_t i = out_parentheses_count;
                out_parentheses[i].opening_index = inequality_parentheses_stack[inequality_parentheses_sp].opening_parentheses - str;
                out_parentheses[i].closing_index = p_str - str;
                --inequality_parentheses_sp;

                ++out_parentheses_count;
            }
            break;
                
        default:
            break;
        }

        ++p_str;
    }

    free(pa_parentheses_stacks);
    pa_parentheses_stacks = NULL;

    qsort(out_parentheses, out_parentheses_count, sizeof(parenthesis_t), compare_parenthesis);
    return out_parentheses_count;
}
