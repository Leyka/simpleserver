#include <stdlib.h>
#include "router.h"
#include "routing_table.h"
#include "types.h"

RoutingTable* routes_get;
RoutingTable* routes_post;

void router_init()
{
    routes_get = rt_init();
    routes_post = rt_init();
}

void router_register(HTTP_Method method, char *path, http_func_t func)
{
    if (method == GET) {
        rt_insert(routes_get, path, func);
    }
    else if (method == POST) {
        rt_insert(routes_post, path, func);
    }
}

http_func_t router_get_func(HTTP_Method method, char *path)
{
    if (method == GET) {
        return rt_search(routes_get, path);
    }

    if (method == POST) {
        return rt_search(routes_post, path);
    }

    return NULL;
}

void router_free()
{
    rt_free(routes_get);
    rt_free(routes_post);
}