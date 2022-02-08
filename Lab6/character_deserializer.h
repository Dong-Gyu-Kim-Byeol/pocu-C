#ifndef CHARACTER_DESERIALIZER_H
#define CHARACTER_DESERIALIZER_H

#include <stddef.h>

enum {
    MAX_MINION_NAME_LENGTH = 50,
    MAX_CHARACTER_V3_NAME_LENGTH = 50,
    MAX_CHARACTER_V3_MINION_COUNT = 3
};

typedef struct {
    unsigned int health;
    unsigned int strength;
    unsigned int defence;
    char name[MAX_MINION_NAME_LENGTH + 1];
} minion_t;

typedef struct {
    unsigned int fire;
    unsigned int cold;
    unsigned int lightning;
} elemental_resistance_t;

typedef struct {
    unsigned int level;
    unsigned int health;
    unsigned int mana;
    unsigned int strength;
    unsigned int dexterity;
    unsigned int intelligence;
    unsigned int armour;
    unsigned int evasion;
    unsigned int leadership;
    elemental_resistance_t elemental_resistance;

    size_t minion_count;
    minion_t minions[MAX_CHARACTER_V3_MINION_COUNT];

    char name[MAX_CHARACTER_V3_NAME_LENGTH + 1];
} character_v3_t;

/* required */
int get_character(const char* filename, character_v3_t* out_character);


/* my function */
int is_character_v1(FILE* const character_v1_file);
int is_character_v2(FILE* const character_v2_file);
int is_character_v3(FILE* const character_v3_file);


#endif /* CHARACTER_DESERIALIZER_H */
