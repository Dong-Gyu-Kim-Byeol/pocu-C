#include "todo_list.h"

#include <stdlib.h>
#include <memory.h>

todo_list_t init_todo_list(size_t max_size)
{
    todo_list_t todo_list = { 0, };
    todo_list.capacity = max_size;

    return todo_list;
}

void finalize_todo_list(todo_list_t* todo_list)
{
    while (todo_list->list != NULL) {
        task_t* next = todo_list->list->next;

        free(todo_list->list);
        todo_list->list = next;
    }

    memset(todo_list, 0, sizeof(todo_list_t));
}

bool add_todo(todo_list_t* todo_list, const int32_t priority, const char* task)
{
    if (todo_list->count >= todo_list->capacity) {
        return false;
    }

    task_t* new_node = malloc(sizeof(task_t));
    new_node->next = NULL;
    new_node->priority = priority;
    memcpy(new_node->str, task, sizeof(new_node->str));
    new_node->str[sizeof(new_node->str) - 1] = '\0';

    task_t** pp = &(todo_list->list);
    while (*pp != NULL) {
        if ((*pp)->priority < new_node->priority) {
            break;
        }

        pp = &(*pp)->next;
    }

    new_node->next = *pp;
    *pp = new_node;

    ++todo_list->count;
    return true;
}

bool complete_todo(todo_list_t* todo_list)
{
    if (todo_list->count <= 0) {
        return false;
    }

    task_t* next = todo_list->list->next;
    free(todo_list->list);
    todo_list->list = next;

    --todo_list->count;
    return true;
}

const char* peek_or_null(const todo_list_t* todo_list)
{
    if (todo_list->count <= 0) {
        return NULL;
    }

    return todo_list->list->str;
}

size_t get_count(const todo_list_t* todo_list)
{
    return todo_list->count;
}

bool is_empty(const todo_list_t* todo_list)
{
    return todo_list->count == 0;
}

