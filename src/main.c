#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "network.h"

#define PORT "3000"

// Check and respond to incoming connections
void main_loop(int listener_sockfd)
{
    int main_sockfd;
    struct sockaddr_storage client_addr;
    socklen_t addr_size;

    while (1) {
        addr_size = sizeof client_addr;
        main_sockfd = accept(listener_sockfd, (struct sockaddr *) &client_addr, &addr_size);
        if (main_sockfd == -1) {
            continue;
        }

        // Someone is connected.
        puts("Gotcha.");
    }
}

int main()
{
    int listener_sockfd = create_listener_socket(PORT);
    if (listener_sockfd == -1) {
        exit(1);
    }

    printf("Listening on port %s ...\n", PORT);

    main_loop(listener_sockfd);
}