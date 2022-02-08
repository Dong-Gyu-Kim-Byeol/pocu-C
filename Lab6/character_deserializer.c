#define _CRT_SECURE_NO_WARNINGS


#include <assert.h>
#include <stdio.h>
#include <string.h>


#include "my_string.h"
#include "character_deserializer.h"


#define TRUE (1)
#define FALSE (0)
#define LINE_SIZE (512)

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define MAX_CHARACTER_V2_NAME_LENGTH (50)


typedef struct {
    unsigned int level;
    unsigned int health;
    unsigned int mana;
    unsigned int strength;
    unsigned int dexterity;
    unsigned int intelligence;
    unsigned int defence;
    unsigned int id;
} character_v1_t;

typedef struct {
    unsigned int level;
    unsigned int health;
    unsigned int mana;
    unsigned int strength;
    unsigned int dexterity;
    unsigned int intelligence;
    unsigned int armour;
    unsigned int evasion;
    unsigned int magic_resistance;
    char name[MAX_CHARACTER_V2_NAME_LENGTH + 1];
} character_v2_t;

static int is_character_v1_get_strcut(FILE* const character_v1_file, character_v1_t* out_character_v1_or_null);
static int is_character_v2_get_strcut(FILE* const character_v2_file, character_v2_t* out_character_v2_or_null);
static int is_character_v3_get_strcut(FILE* const character_v3_file, character_v3_t* out_character_v3_or_null);

static void convert_character_v1_to_character_v2(const character_v1_t* const character_v1, character_v2_t* const out_character_v2);
static void convert_character_v2_to_character_v3(const character_v2_t* const character_v2, character_v3_t* const out_character_v3);


/* required */
int get_character(const char* filename, character_v3_t* out_character)
{
    int ret_version = -1;

    FILE* character_file = fopen(filename, "r");
    if (character_file == NULL) {
        return -1;
    }

    {
        character_v1_t character_v1;
        character_v2_t character_v2;

        if (is_character_v1_get_strcut(character_file, &character_v1) == TRUE) {
            convert_character_v1_to_character_v2(&character_v1, &character_v2);
            convert_character_v2_to_character_v3(&character_v2, out_character);

            ret_version = 1;
            goto ret;
        }
    }

    {
        character_v2_t character_v2;

        if (is_character_v2_get_strcut(character_file, &character_v2) == TRUE) {
            convert_character_v2_to_character_v3(&character_v2, out_character);

            ret_version = 2;
            goto ret;
        }
    }

    {
        if (is_character_v3_get_strcut(character_file, out_character) == TRUE) {
            ret_version = 3;
            goto ret;
        }
    }

ret:
    fclose(character_file);
    return ret_version;
}


/* my function */
int is_character_v1(FILE* const character_v1_file)
{
    return is_character_v1_get_strcut(character_v1_file, NULL);
}

int is_character_v1_get_strcut(FILE* const character_v1_file, character_v1_t* out_character_v1_or_null)
{
    const size_t CHARACTER_V1_ARG_COUNT = 8;

    const char DELIMS[] = ":,";
    char line[LINE_SIZE];
    const char* fgets_ret;

    fseek(character_v1_file, 0, SEEK_SET);

    {
        char* arg_name_token;
        char* arg_value_token;
        char* next_token_start_point;
        size_t i;

        fgets_ret = fgets(line, sizeof(line), character_v1_file);
        if (fgets_ret == NULL) {
            return FALSE;
        }
        next_token_start_point = line;

        for (i = 0; i < CHARACTER_V1_ARG_COUNT; ++i) {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
            if (arg_name_token == NULL) {
                return FALSE;
            }

            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (arg_value_token == NULL) {
                return FALSE;
            }

            switch (*arg_name_token) {
            case 'i':
                if (strcmp("id", arg_name_token) == 0) {
                    if (out_character_v1_or_null != NULL) {
                        out_character_v1_or_null->id = str_to_uint(arg_value_token);
                    }
                } else if (strcmp("intel", arg_name_token) == 0) {
                    if (out_character_v1_or_null != NULL) {
                        out_character_v1_or_null->intelligence = str_to_uint(arg_value_token);
                    }
                } else {
                    return FALSE;
                }
                break;

            case 'l':
                if (strcmp("lvl", arg_name_token) != 0) {
                    return FALSE;
                }

                if (out_character_v1_or_null != NULL) {
                    out_character_v1_or_null->level = str_to_uint(arg_value_token);
                }
                break;

            case 's':
                if (strcmp("str", arg_name_token) != 0) {
                    return FALSE;
                }

                if (out_character_v1_or_null != NULL) {
                    out_character_v1_or_null->strength = str_to_uint(arg_value_token);
                }
                break;

            case 'd':
                if (strcmp("dex", arg_name_token) == 0) {
                    if (out_character_v1_or_null != NULL) {
                        out_character_v1_or_null->dexterity = str_to_uint(arg_value_token);
                    }
                } else if (strcmp("def", arg_name_token) == 0) {
                    if (out_character_v1_or_null != NULL) {
                        out_character_v1_or_null->defence = str_to_uint(arg_value_token);
                    }
                } else {
                    return FALSE;
                }
                break;

            case 'h':
                if (strcmp("hp", arg_name_token) != 0) {
                    return FALSE;
                }

                if (out_character_v1_or_null != NULL) {
                    out_character_v1_or_null->health = str_to_uint(arg_value_token);
                }
                break;

            case 'm':
                if (strcmp("mp", arg_name_token) != 0) {
                    return FALSE;
                }

                if (out_character_v1_or_null != NULL) {
                    out_character_v1_or_null->mana = str_to_uint(arg_value_token);
                }
                break;

            default:
                return FALSE;
                break;
            }
        }

        tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
        if (arg_name_token != NULL) {
            return FALSE;
        }
    }

    fgets_ret = fgets(line, sizeof(line), character_v1_file);
    if (fgets_ret != NULL) {
        return FALSE;
    }

    /*printf("level : %u\n", out_character_v1_or_null->level);
    printf("health : %u\n", out_character_v1_or_null->health);
    printf("mana : %u\n", out_character_v1_or_null->mana);
    printf("strength : %u\n", out_character_v1_or_null->strength);
    printf("dexterity : %u\n", out_character_v1_or_null->dexterity);
    printf("intelligence : %u\n", out_character_v1_or_null->intelligence);
    printf("defence : %u\n", out_character_v1_or_null->defence);
    printf("id : %u\n", out_character_v1_or_null->id);*/

    return TRUE;
}

int is_character_v2(FILE* const character_v2_file)
{
    return is_character_v2_get_strcut(character_v2_file, NULL);
}

int is_character_v2_get_strcut(FILE* const character_v2_file, character_v2_t* out_character_v2_or_null)
{
    const char* CHARACTER_V2_ARG_NAMES[] = {
        "name",
        "level",
        "strength",
        "dexterity",
        "intelligence",
        "armour",
        "evasion",
        "magic_resistance",
        "health",
        "mana"
    };

    const char DELIMS[] = ",\n";
    char line[LINE_SIZE];
    const char* fgets_ret;

    fseek(character_v2_file, 0, SEEK_SET);

    /* check arg names */
    {
        const char** p_character_v2_arg_names = CHARACTER_V2_ARG_NAMES;
        char* arg_name_token;
        char* next_token_start_point;
        size_t i;

        fgets_ret = fgets(line, sizeof(line), character_v2_file);
        if (fgets_ret == NULL) {
            return FALSE;
        }
        next_token_start_point = line;

        for (i = 0; i < ARRAY_LEN(CHARACTER_V2_ARG_NAMES); ++i) {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
            if (strcmp(arg_name_token, *p_character_v2_arg_names) != 0) {
                return FALSE;
            }

            ++p_character_v2_arg_names;
        }

        tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
        if (arg_name_token != NULL) {
            return FALSE;
        }
    }

    /* copy arg valeus */
    {
        char* arg_value_token;
        char* next_token_start_point;

        fgets_ret = fgets(line, sizeof(line), character_v2_file);
        if (fgets_ret == NULL) {
            return FALSE;
        }
        next_token_start_point = line;

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                strncpy(out_character_v2_or_null->name, arg_value_token, MAX_CHARACTER_V2_NAME_LENGTH + 1);
                out_character_v2_or_null->name[MAX_CHARACTER_V2_NAME_LENGTH] = '\0';
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->level = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->strength = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->dexterity = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->intelligence = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->armour = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->evasion = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->magic_resistance = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->health = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v2_or_null != NULL) {
                out_character_v2_or_null->mana = str_to_uint(arg_value_token);
            }
        }

        tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
        if (arg_value_token != NULL) {
            return FALSE;
        }
    }

    fgets_ret = fgets(line, sizeof(line), character_v2_file);
    if (fgets_ret != NULL) {
        return FALSE;
    }

    return TRUE;
}

int is_character_v3(FILE* const character_v3_file)
{
    return is_character_v3_get_strcut(character_v3_file, NULL);
}

int is_character_v3_get_strcut(FILE* const character_v3_file, character_v3_t* out_character_v3_or_null)
{
    const char* CHARACTER_V3_ARG_NAMES[] = {
        "name",
        "level",
        "health",
        "mana",
        "strength",
        "dexterity",
        "intelligence",
        "armour",
        "evasion",
        "fire_res",
        "cold_res",
        "lightning_res",
        "leadership",
        "minion_count"
    };

    const char* MINION_ARG_NAMES[] = {
        "name",
        "health",
        "strength",
        "defence"
    };

    const char DELIMS[] = "| \n";

    const char* fgets_ret;
    char line[LINE_SIZE];
    size_t minion_count = 0;
    size_t i;

    fseek(character_v3_file, 0, SEEK_SET);

    /* check character arg names */
    {
        const char** p_character_v3_arg_names = CHARACTER_V3_ARG_NAMES;
        char* arg_name_token;
        char* next_token_start_point;

        fgets_ret = fgets(line, sizeof(line), character_v3_file);
        if (fgets_ret == NULL) {
            return FALSE;
        }
        next_token_start_point = line;

        for (i = 0; i < ARRAY_LEN(CHARACTER_V3_ARG_NAMES); ++i) {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
            if (strcmp(arg_name_token, *p_character_v3_arg_names) != 0) {
                return FALSE;
            }

            ++p_character_v3_arg_names;
        }

        tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
        if (arg_name_token != NULL) {
            return FALSE;
        }
    }

    /* copy character arg valeus */
    {
        char* arg_value_token;
        char* next_token_start_point;

        fgets_ret = fgets(line, sizeof(line), character_v3_file);
        if (fgets_ret == NULL) {
            return FALSE;
        }
        next_token_start_point = line;

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                strncpy(out_character_v3_or_null->name, arg_value_token, MAX_CHARACTER_V3_NAME_LENGTH + 1);
                out_character_v3_or_null->name[MAX_CHARACTER_V3_NAME_LENGTH] = '\0';
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->level = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->health = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->mana = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->strength = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->dexterity = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->intelligence = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->armour = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->evasion = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->elemental_resistance.fire = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->elemental_resistance.cold = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->elemental_resistance.lightning = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->leadership = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            minion_count = str_to_uint(arg_value_token);
            if (minion_count > MAX_CHARACTER_V3_MINION_COUNT) {
                return FALSE;
            }

            if (out_character_v3_or_null != NULL) {
                out_character_v3_or_null->minion_count = minion_count;
            }
        }

        tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
        if (arg_value_token != NULL) {
            return FALSE;
        }

        if (minion_count == 0) {
            goto file_end;
        }
    }

    /* check minion arg names */
    {
        const char** p_minion_arg_names = MINION_ARG_NAMES;
        char* arg_name_token;
        char* next_token_start_point;
        size_t i;

        fgets_ret = fgets(line, sizeof(line), character_v3_file);
        if (fgets_ret == NULL) {
            return FALSE;
        }
        next_token_start_point = line;

        for (i = 0; i < ARRAY_LEN(MINION_ARG_NAMES); ++i) {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
            if (strcmp(arg_name_token, *p_minion_arg_names) != 0) {
                return FALSE;
            }

            ++p_minion_arg_names;
        }

        tokenize_or_null(next_token_start_point, DELIMS, &arg_name_token, &next_token_start_point);
        if (arg_name_token != NULL) {
            return FALSE;
        }
    }

    /* copy minion arg valeus */
    for (i = 0; i < minion_count; ++i) {
        char* arg_value_token;
        char* next_token_start_point;
        minion_t* const p_out_minions_or_null = out_character_v3_or_null == NULL ? NULL : out_character_v3_or_null->minions + i;

        fgets_ret = fgets(line, sizeof(line), character_v3_file);
        if (fgets_ret == NULL) {
            return FALSE;
        }
        next_token_start_point = line;

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (p_out_minions_or_null != NULL) {
                strncpy(p_out_minions_or_null->name, arg_value_token, MAX_MINION_NAME_LENGTH + 1);
                p_out_minions_or_null->name[MAX_MINION_NAME_LENGTH] = '\0';
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (p_out_minions_or_null != NULL) {
                p_out_minions_or_null->health = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (p_out_minions_or_null != NULL) {
                p_out_minions_or_null->strength = str_to_uint(arg_value_token);
            }
        }

        {
            tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
            if (p_out_minions_or_null != NULL) {
                p_out_minions_or_null->defence = str_to_uint(arg_value_token);
            }
        }

        tokenize_or_null(next_token_start_point, DELIMS, &arg_value_token, &next_token_start_point);
        if (arg_value_token != NULL) {
            return FALSE;
        }
    }

file_end:
    fgets_ret = fgets(line, sizeof(line), character_v3_file);
    if (fgets_ret != NULL) {
        return FALSE;
    }

    return TRUE;
}

void convert_character_v1_to_character_v2(const character_v1_t* const character_v1, character_v2_t* const out_character_v2)
{
    const char NAME_FIRST_STRING[] = "player_";
    assert(ARRAY_LEN(NAME_FIRST_STRING) <= MAX_CHARACTER_V2_NAME_LENGTH + 1);

    /* == */
    out_character_v2->level = character_v1->level;
    out_character_v2->health = character_v1->health;
    out_character_v2->mana = character_v1->mana;
    out_character_v2->strength = character_v1->strength;
    out_character_v2->dexterity = character_v1->dexterity;
    out_character_v2->intelligence = character_v1->intelligence;

    /* change name */
    out_character_v2->armour = character_v1->defence;

    /* new arg */
    out_character_v2->evasion = out_character_v2->dexterity / 2;
    out_character_v2->magic_resistance = out_character_v2->armour / 4;
    {
        char* const p_name = out_character_v2->name;
        strcpy(p_name, NAME_FIRST_STRING);

        assert(*(p_name + ARRAY_LEN(NAME_FIRST_STRING) - 1) == '\0');
        *(p_name + ARRAY_LEN(NAME_FIRST_STRING) - 1) = 'p';
        uint_to_str(p_name + ARRAY_LEN(NAME_FIRST_STRING) - 1, MAX_CHARACTER_V2_NAME_LENGTH + 1 - ARRAY_LEN(NAME_FIRST_STRING), character_v1->id);
    }
}

void convert_character_v2_to_character_v3(const character_v2_t* const character_v2, character_v3_t* const out_character_v3)
{
    assert(MAX_CHARACTER_V2_NAME_LENGTH + 1 <= MAX_CHARACTER_V3_NAME_LENGTH + 1);

    /* == */
    out_character_v3->level = character_v2->level;
    out_character_v3->health = character_v2->health;
    out_character_v3->mana = character_v2->mana;
    out_character_v3->strength = character_v2->strength;
    out_character_v3->dexterity = character_v2->dexterity;
    out_character_v3->intelligence = character_v2->intelligence;
    out_character_v3->armour = character_v2->armour;
    out_character_v3->evasion = character_v2->evasion;
    strcpy(out_character_v3->name, character_v2->name);

    /* new arg */
    out_character_v3->leadership = out_character_v3->level / 10;
    out_character_v3->elemental_resistance.fire = character_v2->magic_resistance / 3;
    out_character_v3->elemental_resistance.cold = character_v2->magic_resistance / 3;
    out_character_v3->elemental_resistance.lightning = character_v2->magic_resistance / 3;
    out_character_v3->minion_count = 0;
}
