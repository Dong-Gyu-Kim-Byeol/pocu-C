#include <assert.h>
#include <string.h>

#include "my_string.h"

#define _CRT_SECURE_NO_WARNINGS

int main(void)
{
    const char* const str = "   We   all   live   in a yellow submarine";
    char str_cpy[43];
    const char* token;
    
    strncpy(str_cpy, str, 42);
    str_cpy[42] = '\0';
    
    reverse(str_cpy);
    assert(strcmp("enirambus wolley a ni   evil   lla   eW   ", str_cpy) == 0);

    assert(index_of(str, "all ") == 8);
    assert(index_of(str, "marine") == 36);
    assert(index_of(str, "all good") == -1);
    assert(index_of(str, "marinett") == -1);
    assert(index_of(str, "") == 0);
    
    reverse(str_cpy);
    assert(strcmp(str_cpy, str) == 0);

    reverse_by_words(str_cpy);
    assert(strcmp("   eW   lla   evil   ni a wolley enirambus", str_cpy) == 0);

    reverse_by_words(str_cpy);
    assert(strcmp(str_cpy, str) == 0);
    
    assert(tokenize(NULL, " ") == NULL);
    
    token = tokenize(str_cpy, " ");
    assert(token == str_cpy + 3); 
    assert(strcmp(token, "We") == 0);

    token = tokenize(NULL, " ");
    assert(token == str_cpy + 8);
    assert(strcmp(token, "all") == 0);

    token = tokenize(NULL, " ");
    assert(token == str_cpy + 14);
    assert(strcmp(token, "live") == 0);

    token = reverse_tokenize(NULL, " ");
    assert(token == str_cpy + 21);
    assert(strcmp(token, "ni") == 0);

    token = reverse_tokenize(NULL, " ");
    token = reverse_tokenize(NULL, " ");
    assert(token == str_cpy + 26);
    assert(strcmp(token, "wolley") == 0);

    token = reverse_tokenize(NULL, " ");
    assert(token == str_cpy + 33);
    assert(strcmp(token, "enirambus") == 0);

    token = reverse_tokenize(NULL, " ");
    assert(token == NULL);

    token = reverse_tokenize(NULL, " ");
    assert(token == NULL);

    return 0;
}
