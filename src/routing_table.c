/*
 * Routing table is just a hash table.
 * It keeps the route "path" as key,
 * and the associated function pointer as value.
 *
 * Reference: https://github.com/jamesroutley/write-a-hash-table
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "routing_table.h"

RoutingTable* rt_init()
{
    RoutingTable* rt = malloc(sizeof(RoutingTable));
    rt->count = 0;
    rt->size = RT_DEFAULT_SIZE;
    rt->items = calloc((size_t)rt->size, sizeof(RT_Item));

    return rt;
}

static RT_Item* rt_new_item(char* key, http_func_t func)
{
    RT_Item* item = malloc(sizeof(RT_Item));
    item->key = strdup(key);
    item->func = func;

    return item;
}

static void rt_remove(RT_Item* item)
{
    free(item->key);
    free(item->func);
    free(item);
}

/*
 * Hashing algorithm
 * Find an index key (hash) of the given string depending of the prime number and size.
 * str: string to hash
 * prime_number: prime number which is larger than 128 (max ASCII)
 * size: size of the hash table
 */
static int rt_hash(char* str, int prime_number, int size)
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
static int rt_get_hash(char* str, int size, int attempt)
{
    int hash_a = rt_hash(str, 131, size);
    int hash_b = rt_hash(str, 157, size);

    return (hash_a + (attempt * (hash_b + 1))) % size;
}

// Insert new item in the first empty bucket
void rt_insert(RoutingTable* rt, char* key, http_func_t func)
{
    RT_Item* item = rt_new_item(key, func);
    int index = rt_get_hash(item->key, rt->size, 0);
    RT_Item* curr_item = rt->items[index];
    // Handle collision
    int i = 0;
    while (curr_item != NULL) {
        index = rt_get_hash(item->key, rt->size, ++i);
        curr_item = rt->items[index];
    }
    // curr_item == NULL, we're good to go.
    rt->items[index] = item;
    rt->count++;
}

http_func_t rt_search(RoutingTable* rt, char* key)
{

}

void rt_delete(RoutingTable* rt, char* key)
{

}

void rt_free(RoutingTable* rt)
{
    for (int i = 0; i < rt->size; ++i) {
        RT_Item* item = rt->items[i];
        if (item != NULL) {
            rt_remove(item);
        }
    }

    free(rt->items);
    free(rt);
}


// !!! TEST ONLY, REMOVE LATER !!!
/*
int main()
{
    RoutingTable* rt = rt_init();
    printf("%p\n", rt->items[1]);
    rt_free(rt);
    printf("%p\n", rt->items[1]); // Seg fault ! which is ok :)
}
 */