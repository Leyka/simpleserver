#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "network.h"
#include "request.h"

#define PORT "3000"

// Check and respond to incoming connections
void serve(int listener_sockfd)
{
    int new_sockfd;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;

    while (1) {
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

        // TODO
        // Look for existing routes
        // If exists, send response (or map route to a function and call it?)
        // If not, send 404

        clear_request(new_sockfd, req);
    }
}

int main()
{
    int listener_sockfd = create_listener_socket(PORT);
    if (listener_sockfd == -1) {
        exit(1);
    }

    printf("Listening on port %s ...\n", PORT);
    serve(listener_sockfd);
}