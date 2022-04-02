#define _CRT_SECURE_NO_WARNINGS

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>


#include "hashmap.h"
static node_t* get_node_or_null(const hashmap_t* hashmap, const char* key);
static void destroy_linked_list(node_t** phead);

hashmap_t* init_hashmap_malloc(size_t length, size_t(*p_hash_func)(const char* key))
{
    const plist_size = sizeof(node_t*) * (length + 1);

    hashmap_t* pa_hashmap = malloc(sizeof(hashmap_t));
    memset(pa_hashmap, 0, sizeof(*pa_hashmap));

    pa_hashmap->length = length;
    pa_hashmap->hash_func = p_hash_func;

    pa_hashmap->plist = malloc(plist_size);
    memset(pa_hashmap->plist, 0, plist_size);

    return pa_hashmap;
}

int add_key(hashmap_t* hashmap, const char* key, const int value)
{
    node_t* pa_new_node;

    if (HASHMAP_NO_VALUE == value) {
        return FALSE;
    }
    if (NULL != get_node_or_null(hashmap, key)) {
        return FALSE;
    }

    /* make */
    {
        const size_t key_len = strlen(key);

        pa_new_node = malloc(sizeof(node_t));
        memset(pa_new_node, 0, sizeof(*pa_new_node));

        pa_new_node->key = malloc(key_len);
        strcpy(pa_new_node->key, key);

        pa_new_node->value = value;
    }

    /* add */
    {
        const size_t hash = hashmap->hash_func(key);
        const size_t index = hash % hashmap->length;

        node_t** phead = &hashmap->plist[index];
        pa_new_node->next = *phead;
        *phead = pa_new_node;
        return TRUE;
    }
}

int get_value(const hashmap_t* hashmap, const char* key)
{
    node_t* node = get_node_or_null(hashmap, key);
    if (NULL == node) {
        return HASHMAP_NO_VALUE;
    }

    return node->value;
}

int update_value(hashmap_t* hashmap, const char* key, const int value)
{
    node_t* node = get_node_or_null(hashmap, key);
    if (NULL == node) {
        return FALSE;
    }

    node->value = value;
    return TRUE;
}

int remove_key(hashmap_t* hashmap, const char* key)
{
    const size_t hash = hashmap->hash_func(key);
    const size_t index = hash % hashmap->length;

    node_t** pp_node = &hashmap->plist[index];
    while (NULL != *pp_node) {
        node_t* node = *pp_node;
        if (strcmp(node->key, key) == 0) {
            *pp_node = node->next;

            free(node->key);
            node->key = NULL;
            free(node);
            node = NULL;
            return TRUE;
        }

        pp_node = &(*pp_node)->next;
    }

    return FALSE;
}

void destroy(hashmap_t* hashmap)
{
    node_t** p_plist = hashmap->plist;
    while (NULL != *p_plist) {
        node_t** phead = p_plist;
        destroy_linked_list(phead);
        assert(NULL == *phead);

        ++p_plist;
    }

    free(hashmap->plist);
    memset(hashmap, 0, sizeof(*hashmap));
}



node_t* get_node_or_null(const hashmap_t* hashmap, const char* key)
{
    const size_t hash = hashmap->hash_func(key);
    const size_t index = hash % hashmap->length;

    node_t* node = hashmap->plist[index];
    while (NULL != node) {
        if (strcmp(node->key, key) == 0) {
            return node;
        }

        node = node->next;
    }

    return NULL;
}

void destroy_linked_list(node_t** phead)
{
    node_t* head = *phead;
    while (head != NULL) {
        node_t* next = head->next;

        free(head->key);
        head->key = NULL;

        free(head);
        head = next;
    }

    *phead = NULL;
}
