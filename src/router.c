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
    switch (method) {
        case GET:
            rt_insert(routes_get, path, func);
            break;
        case POST:
            rt_insert(routes_post, path, func);
            break;
        default:
            break;
    }
}

http_func_t router_get_func(HTTP_Method method, char *path)
{
    switch (method) {
        case GET:
            return rt_search(routes_get, path);
        case POST:
            return rt_search(routes_post, path);
        default:
            return NULL;
    }
}

void router_free()
{
    rt_free(routes_get);
    rt_free(routes_post);
}