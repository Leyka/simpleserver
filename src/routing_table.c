/*
 * Routing table has the same implementation of a hash table.
 * It keeps the route "path" as key, and the associated function pointer as value.
 *
 * Reference: https://github.com/jamesroutley/write-a-hash-table
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "routing_table.h"
#include "utils/math.h"

static RT_Item RT_DELETED_ITEM = {NULL, NULL};

static RoutingTable* rt_new_sized(int base_size)
{
    RoutingTable* rt = malloc(sizeof(RoutingTable));
    rt->base_size = base_size;
    rt->size = next_prime(base_size);
    rt->count = 0;
    rt->items = calloc((size_t)rt->size, sizeof(RT_Item));

    return rt;
}

RoutingTable* rt_init()
{
    return rt_new_sized(RT_DEFAULT_SIZE);
}

static RT_Item* rt_new_item(char *key, http_func_t func)
{
    RT_Item *item = malloc(sizeof(RT_Item));
    item->key = strdup(key);
    item->func = func;

    return item;
}

static void rt_free_item(RT_Item *item)
{
    free(item->key);
    free(item->func);
    free(item);
}

// Resize routing table
static void rt_resize(RoutingTable *rt, int base_size)
{
    if (base_size < RT_DEFAULT_SIZE) {
        return;
    }

    // Create new temporary table with the wanted size
    // We only keep non-NULL and non-deleted elements
    RoutingTable *temp_rt = rt_new_sized(base_size);
    for (int i = 0; i < rt->size; i++) {
        RT_Item *item = rt->items[i];
        if (item != NULL && item != &RT_DELETED_ITEM) {
            rt_insert(temp_rt, item->key, item->func);
        }
    }

    // Replace Real routing table with new sized temporary table before delete it
    rt->base_size = temp_rt->base_size;
    rt->count = temp_rt->count;

    // Swap data between real routing table (rt) and the temporary one
    swap_int(&rt->size, &temp_rt->size);

    RT_Item **temp_items = rt->items;
    rt->items = temp_rt->items;
    temp_rt->items = temp_items;

    // Delete temporary table
    rt_free(temp_rt);
}

static void rt_resize_up(RoutingTable *rt) {
    int new_size = rt->base_size * 2;
    rt_resize(rt, new_size);
}


static void rt_resize_down(RoutingTable *rt) {
    int new_size = rt->base_size / 2;
    rt_resize(rt, new_size);
}

/*
 * Hashing algorithm
 * Find an index key (hash) of the given string depending of the prime number and size.
 * str: string to hash
 * prime_number: prime number which is larger than 128 (max ASCII)
 * size: size of the hash table
 */
static int rt_hash(char *str, int prime_number, int size)
{
    long hash = 0;
    size_t len_s = strlen(str);

    for (int i = 0; i < len_s; ++i) {
        hash += (long) pow(prime_number, len_s - (i+1)) * str[i];
    }
    hash = hash % size;

    return (int)hash;
}

// Returns hash while handling collision using
// open addressing with double hashing
static int rt_get_hash(char *str, int size, int attempt)
{
    int hash_a = rt_hash(str, 131, size);
    int hash_b = rt_hash(str, 157, size);

    return (hash_a + (attempt * (hash_b + 1))) % size;
}

// Insert new item in the first empty bucket
void rt_insert(RoutingTable *rt, char *key, http_func_t func)
{
    // Check if we need to resize
    int load = rt->count * 100 / rt->size;
    if (load > 70) {
        rt_resize_up(rt);
    }

    int iteration = 0;
    RT_Item* item = rt_new_item(key, func);
    int index = rt_get_hash(item->key, rt->size, iteration);
    RT_Item* curr_item = rt->items[index];
    // Handle collision
    while (curr_item != NULL) {
        // Update (replace)
        if (curr_item != &RT_DELETED_ITEM && strcmp(key, curr_item->key) == 0) {
            rt_free_item(curr_item);
            rt->items[index] = item;
            return;
        }
        // Insert
        index = rt_get_hash(item->key, rt->size, ++iteration);
        curr_item = rt->items[index];
    }
    // curr_item == NULL, we're good to go.
    rt->items[index] = item;
    rt->count++;
}

http_func_t rt_search(RoutingTable* rt, char* key)
{
    int iteration = 0;
    int index = rt_get_hash(key, rt->size, iteration);
    RT_Item *item = rt->items[index];

    while (item != NULL) {
        if (item != &RT_DELETED_ITEM && strcmp(key, item->key) == 0) {
            return item->func;
        }
        // Handle collision
        index = rt_get_hash(key, rt->size, ++iteration);
        item = rt->items[index];
    }

    return NULL;
}

void rt_delete(RoutingTable *rt, char *key)
{
    // Check if we need to downsize
    int load = rt->count * 100 / rt->size;
    if (load < 10) {
        rt_resize_down(rt);
    }

    // We actually don't delete, we just replace item by NULLs
    // If we really delete, we will break the open addressed chain
    int iteration = 0;
    int index = rt_get_hash(key, rt->size, iteration);
    RT_Item *item = rt->items[index];

    while(item != NULL) {
        if (item !=  &RT_DELETED_ITEM) {
            if (strcmp(key, item->key) == 0) {
                // Free resources and mark it as deleted
                rt_free_item(item);
                rt->items[index] = &RT_DELETED_ITEM;
            }
        }
        // Continue iteration
        index = rt_get_hash(key, rt->size, ++iteration);
        item = rt->items[index];
    }

    rt->count--;
}

void rt_free(RoutingTable *rt)
{
    for (int i = 0; i < rt->size; ++i) {
        RT_Item *item = rt->items[i];
        if (item != NULL) {
            rt_free_item(item);
        }
    }

    free(rt->items);
    free(rt);
}