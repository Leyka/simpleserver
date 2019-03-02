#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "network.h"

#define PORT "3000"

int main()
{
    int listener_sockfd = create_listener_socket(PORT);
    if (listener_sockfd == -1) {
        exit(1);
    }

    printf("Listening on port %s ...\n", PORT);
    return 0;
}