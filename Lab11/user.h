#ifndef USER_H
#define USER_H


#define MAX_STR_LENGTH (50)

typedef struct user {
    size_t id;
    char username[MAX_STR_LENGTH + 1];
    char email[MAX_STR_LENGTH + 1];
    char password[MAX_STR_LENGTH + 1];
} user_t;


#endif  // USER_H
