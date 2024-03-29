#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <string.h>
#include <stdio.h>

#include "data_store.h"


static FILE* open_log_file(void);
static void close_log_file(FILE* log_file);

static void get_hided_email(char* const out_hided_email, const char* const plain_email, const size_t out_size);
static void get_hided_password(char* const out_hided_password, const char* const plain_password, const size_t out_size);

static void log_update_email(const user_t* const pre_update_user, const char* const new_email);
static void log_update_password(const user_t* const pre_update_user, const char* const new_password);


static FILE* open_log_file(void)
{
    const char LOG_FILE_MODE[] = "ab";
    const char LOG_FILE_NAME[] = "log.txt";

    FILE* log_file = fopen(LOG_FILE_NAME, LOG_FILE_MODE);
    assert(NULL != log_file);

    return log_file;
}

static void close_log_file(FILE* log_file)
{
    fclose(log_file);
    log_file = NULL;
}

void get_hided_email(char* const out_hided_email, const char* const plain_email, const size_t out_size)
{
#ifdef RELEASE

    if ('\0' == *plain_email) {
        out_hided_email[0] = '\0';
        return;
    }

    char* p_out_str = out_hided_email;
    const char* p_plain_str = plain_email;

    p_out_str[0] = p_plain_str[0];
    ++p_out_str;
    ++p_plain_str;

    while ('@' != *p_plain_str) {
        if ((size_t)(p_out_str - out_hided_email) > out_size - 1) {
            break;
        }

        *p_out_str = '*';

        ++p_out_str;
        ++p_plain_str;
    }
    *(p_out_str - 1) = *(p_plain_str - 1);

    if (1 == p_plain_str - plain_email) {
        out_hided_email[0] = '*';
    } else if (2 == p_plain_str - plain_email) {
        out_hided_email[0] = plain_email[0];
        out_hided_email[1] = '*';
    }
    
    strncpy(p_out_str, p_plain_str, out_size - (p_out_str - out_hided_email));
    out_hided_email[out_size - 1] = '\0';
    return;

#else

    strncpy(out_hided_email, plain_email, out_size);
    out_hided_email[out_size - 1] = '\0';
    return;

#endif
}

void get_hided_password(char* const out_hided_password, const char* const plain_password, const size_t out_size)
{
#ifdef RELEASE

    if ('\0' == *plain_password) {
        out_hided_password[0] = '\0';
        return;
    }

    char* p_out_str = out_hided_password;
    const char* p_plain_str = plain_password;

    p_out_str[0] = p_plain_str[0];
    ++p_out_str;
    ++p_plain_str;

    while ('\0' != *p_plain_str) {
        if ((size_t)(p_out_str - out_hided_password) > out_size - 1) {
            break;
        }

        *p_out_str = '*';

        ++p_out_str;
        ++p_plain_str;
    }
    *(p_out_str - 1) = *(p_plain_str - 1);
    *(p_out_str) = '\0';

    if (1 == p_plain_str - plain_password) {
        out_hided_password[0] = '*';
    } else if (2 == p_plain_str - plain_password) {
        out_hided_password[0] = plain_password[0];
        out_hided_password[1] = '*';
    }

    strncpy(p_out_str, p_plain_str, out_size - (p_out_str - out_hided_password));
    out_hided_password[out_size - 1] = '\0';
    return;

#else

    strncpy(out_hided_password, plain_password, out_size);
    out_hided_password[out_size - 1] = '\0';
    return;

#endif
}

static void log_update_email(const user_t* const pre_update_user, const char* const new_email)
{
    FILE* log_file = open_log_file();

    char old_str[sizeof(pre_update_user->email)];
    get_hided_email(old_str, pre_update_user->email, sizeof(old_str));

    char new_str[sizeof(pre_update_user->email)];
    get_hided_email(new_str, new_email, sizeof(new_str));

    fprintf(log_file, "TRACE: User %u updated email from \"%s\" to \"%s\"\n", pre_update_user->id, old_str, new_str);

    close_log_file(log_file);
    log_file = NULL;
}

static void log_update_password(const user_t* const pre_update_user, const char* const new_password)
{
    FILE* log_file = open_log_file();

    char old_str[sizeof(pre_update_user->password)];
    get_hided_password(old_str, pre_update_user->password, sizeof(old_str));

    char new_str[sizeof(pre_update_user->password)];
    get_hided_password(new_str, new_password, sizeof(new_str));

    fprintf(log_file, "TRACE: User %u updated password from \"%s\" to \"%s\"\n", pre_update_user->id, old_str, new_str);

    close_log_file(log_file);
    log_file = NULL;
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
    if ('\0' == *email) {
        return false;
    }

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
    if ('\0' == *password) {
        return false;
    }

    user_t* user = get_user_by_id_or_null(users_or_null, id);
    if (NULL == user) {
        return false;
    }

    log_update_password(user, password);

    strncpy(user->password, password, sizeof(user->password));
    user->password[sizeof(user->password) - 1] = '\0';
    return true;
}
