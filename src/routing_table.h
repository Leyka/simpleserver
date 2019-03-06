#ifndef WEBSERVER_ROUTING_TABLE_H
#define WEBSERVER_ROUTING_TABLE_H

#include "types.h"

#define RT_DEFAULT_SIZE 31 // Prime, bigger than needed, to reduce collisions effect

typedef struct RT_Item {
    char *key;
    http_func_t func;
} RT_Item;

typedef struct RoutingTable {
    RT_Item** items;
    int base_size;
    int size; // always a prime number size based on base_size
    int count;
} RoutingTable;

RoutingTable* rt_init();
void rt_insert(RoutingTable* rt, char* key, http_func_t func);
http_func_t rt_search(RoutingTable* rt, char* key);
void rt_delete(RoutingTable* rt, char* key);
void rt_free(RoutingTable* rt);

#endif
