#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include "server.h"
#include "network.h"
#include "request.h"
#include "response.h"
#include "router.h"

volatile int keep_running = 1;

// Declaring global private variables
static int listener_sockfd;
static int new_sockfd;
static struct sockaddr_storage client_addr;
static socklen_t addr_size;

// Catches Ctrl+C (Signal interrupt) from user
static void handle_SIGINT(int _) {
    keep_running = 0;
}

void init_server(char *port)
{
    signal(SIGINT, handle_SIGINT);

    listener_sockfd = create_listener_socket(port);
    if (listener_sockfd == -1) {
        exit(1);
    }

    router_init();

    printf("[!] Server started on port %s\n", port);
}

static void free_server_resources()
{
    router_free();
}

void register_route(HTTP_Method method, char *path, http_func_t func)
{
    router_register(method, path, func);
}

static void handle_request()
{
    addr_size = sizeof client_addr;
    new_sockfd = accept(listener_sockfd, (struct sockaddr *) &client_addr, &addr_size);

    // Someone is connected, analyze request
    Request* req = create_request(new_sockfd);
    if (req == NULL) {
        fprintf(stderr, "Cannot create request");
        clear_request(new_sockfd, req);
        return;
    }

    // Create response
    Response* res = create_response(new_sockfd);

    // Cast method in enum type
    HTTP_Method method = GET;
    if (strcmp(req->method, "POST") == 0) method = POST;

    // Execute function according to route
    http_func_t func_to_execute = router_get_func(method, req->path);
    if (func_to_execute == NULL) {
        send_404(res);
    } else {
        (*func_to_execute)(req, res);
    }

    free_response(res);
    clear_request(new_sockfd, req);
}

void serve_forever()
{
    while (keep_running) {
        handle_request();
    }

    // At this point, we got a SIGINT
    free_server_resources();
}