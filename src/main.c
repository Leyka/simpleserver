#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include  <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "network.h"
#include "request.h"

#define PORT "3000"

volatile bool keep_running = true;

// Catches Ctrl+C (Signal interrupt) from user
void handle_SIGINT(int i) {
    keep_running = false;
}

// Check and respond to incoming connections
void serve(int listener_sockfd)
{
    int new_sockfd;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;

    while (keep_running) {
        addr_size = sizeof client_addr;
        new_sockfd = accept(listener_sockfd, (struct sockaddr *) &client_addr, &addr_size);

        // Someone is connected
        // Analyse request
        Request* req = create_request(new_sockfd);
        if (req == NULL) {
            fprintf(stderr, "Cannot create request");
            clear_request(new_sockfd, req);
            continue;
        }

        printf("Got request! %s %s %s\n", req->method, req->path, req->protocol);

        if (strcmp(req->method, "GET") == 0) {

        }
        else if (strcmp(req->method, "POST") == 0) {

        }
        else {
            // 404
        }

        // TODO
        // Look for existing routes
        // If exists, send response (or map route to a function and call it?)
        // If not, send 404

        /*
         * get_routes = list of { path, pointer to function (req, res) }
         * post_routes
         */

        clear_request(new_sockfd, req);
    }
}

int main()
{
    signal(SIGINT, handle_SIGINT);

    int listener_sockfd = create_listener_socket(PORT);
    if (listener_sockfd == -1) {
        exit(1);
    }

    printf("Listening on port %s ...\n", PORT);
    serve(listener_sockfd);

    // At this point, we exited the main loop because
    // we got A SIGINT
    // TODO: Free memory here
    printf("We will free memory...\n");
}