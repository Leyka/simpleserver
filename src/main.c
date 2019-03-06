#include <stdio.h>
#include "server.h"

#define PORT "3000"

void callback(Request *req, Response *res)
{
    printf("%s from %s \n", req->method, req->path);
}

void register_my_routes()
{
    register_route(GET, "/", &callback);
    register_route(POST, "/", &callback);
}

// Example of server using simpleserver
int main()
{
    init_server(PORT);
    register_my_routes();
    serve_forever();
}