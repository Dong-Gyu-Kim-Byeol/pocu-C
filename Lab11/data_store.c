#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "data_store.h"

static FILE* s_log_file = NULL;


static void init_log_file(void);
static void get_blank_str(char* const out_str, const char* const plain_str, const size_t out_size);
static void log_update_email(const user_t* const pre_update_user, const char* const new_email);
static void log_update_password(const user_t* const pre_update_user, const char* const new_password);


static void init_log_file(void)
{
    const char LOG_FILE_MODE[] = "wb";
    const char LOG_FILE_NAME[] = "log.txt";

#ifdef RELEASE
    //const char LOG_FILE_NAME[] = "log_release.txt";
#else
    //const char LOG_FILE_NAME[] = "log.txt";
#endif

    if (NULL != s_log_file) {
        return;
    }

    s_log_file = fopen(LOG_FILE_NAME, LOG_FILE_MODE);
    if (NULL == s_log_file) {
        printf("faild file open\n");
    }
}

void get_blank_str(char* const out_str, const char* const plain_str, const size_t out_size)
{
#ifdef RELEASE

    if ('\0' == *plain_str) {
        out_str[0] = '\0';
        return;
    }

    char* p_out_str = out_str;
    const char* p_plain_str = plain_str;

    p_out_str[0] = p_plain_str[0];
    ++p_out_str;
    ++p_plain_str;

    while ('@' != *p_plain_str && '\0' != *p_plain_str) {
        *p_out_str = '*';

        ++p_out_str;
        ++p_plain_str;
    }

    if ('@' == *p_plain_str || '\0' == *p_plain_str) {
        *(p_out_str - 1) = *(p_plain_str - 1);
    }

    if (1 == p_plain_str - plain_str) {
        out_str[0] = '*';
    }
    if (2 == p_plain_str - plain_str) {
        out_str[0] = plain_str[0];
        out_str[1] = '*';
    }

    strncpy(p_out_str, p_plain_str, out_size - (p_out_str - out_str));
    out_str[out_size - 1] = '\0';

#else

    strncpy(out_str, plain_str, out_size);
    out_str[out_size - 1] = '\0';

#endif
}

static void log_update_email(const user_t* const pre_update_user, const char* const new_email)
{
    init_log_file();

    char old_str[sizeof(pre_update_user->email)];
    get_blank_str(old_str, pre_update_user->email, sizeof(old_str));

    char new_str[sizeof(pre_update_user->email)];
    get_blank_str(new_str, new_email, sizeof(new_str));

    fprintf(s_log_file, "TRACE: User %zu updated email from \"%s\" to \"%s\"\n", pre_update_user->id, old_str, new_str);
}

static void log_update_password(const user_t* const pre_update_user, const char* const new_password)
{
    init_log_file();

    char old_str[sizeof(pre_update_user->password)];
    get_blank_str(old_str, pre_update_user->password, sizeof(old_str));

    char new_str[sizeof(pre_update_user->email)];
    get_blank_str(new_str, new_password, sizeof(new_str));

    fprintf(s_log_file, "TRACE: User %zu updated password from \"%s\" to \"%s\"\n", pre_update_user->id, old_str, new_str);
}

user_t* get_user_by_id_or_null(user_t** const users_or_null, const size_t id)
{
    if (NULL == users_or_null) {
        return NULL;
    }

    user_t** p_users = users_or_null;
    while (NULL != *p_users) {
        const user_t* user = *p_users;
        if (user->id == id) {
            return (user_t*)user;
        }

        ++p_users;
    }

    return NULL;
}

user_t* get_user_by_username_or_null(user_t** const users_or_null, const char* const username)
{
    if (NULL == users_or_null) {
        return NULL;
    }

    user_t** p_users = users_or_null;
    while (NULL != *p_users) {
        const user_t* user = *p_users;
        if (strncmp(user->username, username, sizeof(user->username)) == 0) {
            return (user_t*)user;
        }

        ++p_users;
    }

    return NULL;
}

bool update_email(user_t** const users_or_null, const size_t id, const char* const email)
{
    user_t* user = get_user_by_id_or_null(users_or_null, id);
    if (NULL == user) {
        return false;
    }

    log_update_email(user, email);

    strncpy(user->email, email, sizeof(user->email));
    user->email[sizeof(user->email) - 1] = '\0';
    return true;
}

bool update_password(user_t** const users_or_null, const size_t id, const char* const password)
{
    user_t* user = get_user_by_id_or_null(users_or_null, id);
    if (NULL == user) {
        return false;
    }

    log_update_password(user, password);

    strncpy(user->password, password, sizeof(user->password));
    user->password[sizeof(user->password) - 1] = '\0';
    return true;
}
