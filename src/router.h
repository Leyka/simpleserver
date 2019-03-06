#ifndef WEBSERVER_ROUTER_H
#define WEBSERVER_ROUTER_H

#include "types.h"

void router_init();
void router_register(HTTP_Method method, char *path, http_func_t func);
http_func_t router_get_func(HTTP_Method method, char *path);
void router_free();

#endif