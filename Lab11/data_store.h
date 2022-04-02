#ifndef DATA_STORE_H
#define DATA_STORE_H

#include <stdbool.h>

#include "user.h"

user_t* get_user_by_id_or_null(user_t** const users_or_null, const size_t id);

user_t* get_user_by_username_or_null(user_t** const users_or_null, const char* const username);

bool update_email(user_t** const users_or_null, const size_t id, const char* const email);

bool update_password(user_t** const users_or_null, const size_t id, const char* const password);

#endif  // DATA_STORE_H
