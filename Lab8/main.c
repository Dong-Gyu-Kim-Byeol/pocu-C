#include <assert.h>
#include <stdio.h>

#include "parentheses.h"

#define PARENTHESES_MAX_SIZE (50)

int main(void)
{
    parenthesis_t parentheses[PARENTHESES_MAX_SIZE];
    
    size_t len = get_matching_parentheses(parentheses, PARENTHESES_MAX_SIZE, "}{)(][><");
    
    assert(len == 0);
    
    len = get_matching_parentheses(parentheses, PARENTHESES_MAX_SIZE, "{{{}}}");

    assert(len == 3);

    assert(parentheses[0].opening_index == 0);
    assert(parentheses[0].closing_index == 5);

    assert(parentheses[1].opening_index == 1);
    assert(parentheses[1].closing_index == 4);

    assert(parentheses[2].opening_index == 2);
    assert(parentheses[2].closing_index == 3);

    len = get_matching_parentheses(parentheses, PARENTHESES_MAX_SIZE, " {]}[[i]{)))} ]");

    assert(len == 4);

    assert(parentheses[0].opening_index == 1);
    assert(parentheses[0].closing_index == 3);

    assert(parentheses[1].opening_index == 4);
    assert(parentheses[1].closing_index == 14);

    assert(parentheses[2].opening_index == 5);
    assert(parentheses[2].closing_index == 7);

    assert(parentheses[3].opening_index == 8);
    assert(parentheses[3].closing_index == 12);

    len = get_matching_parentheses(parentheses, PARENTHESES_MAX_SIZE, "{this}(bracket;[](){[(><>)");

    assert(len == 5);

    assert(parentheses[0].opening_index == 0);
    assert(parentheses[0].closing_index == 5);

    assert(parentheses[1].opening_index == 15);
    assert(parentheses[1].closing_index == 16);

    assert(parentheses[2].opening_index == 17);
    assert(parentheses[2].closing_index == 18);

    assert(parentheses[3].opening_index == 21);
    assert(parentheses[3].closing_index == 25);

    assert(parentheses[4].opening_index == 23);
    assert(parentheses[4].closing_index == 24);

    printf("end main\n");
    return 0;
}
