#ifndef WEBSERVER_SERVER_H
#define WEBSERVER_SERVER_H

#include "types.h"

void init_server(char *port);
static void free_server_resources();
void register_route(HTTP_Method method, char *path, http_func_t func);
void serve_forever();

#endif
