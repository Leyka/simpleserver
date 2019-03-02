/*
 * Network will mostly handle sockets
 * It will create socket based on Host IP adress and given port
 * Basic workflow: Create socket -> bind to Addr/Port -> listen, so:
 * In code:        getaddrinfo() -> socket() -> bind() -> listen()
 *
 * Reference: https://beej.us/guide/bgnet/html/single/bgnet.html
 */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "network.h"

// Listen and returns socket file descriptor
int create_listener_socket(char* port)
{
    int socketfd;
    struct addrinfo *results;

    if (setup_addrinfo(port, &results) == -1) {
        return -1;
    }

    socketfd = init_socket(results);

    freeaddrinfo(results);

    if (socketfd == -1) {
        fprintf(stderr, "Failed to create a socket\n");
        return -1;
    }

    // Start listening for remote computers to connect to this socket/IP
    if (listen(socketfd, NB_CONNECTIONS_ALLOWED) == -1) {
        close(socketfd);
        return -1;
    }

    return socketfd;
}

// Setup and returns linked list of one or more addrinfo structures
int setup_addrinfo(char* port, struct addrinfo **results)
{
    int status;
    struct addrinfo hints;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC; // Either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE; // Bind to host IP address

    status = getaddrinfo(NULL, port, &hints, results);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo() error: %s\n", gai_strerror(status));
        return -1;
    }

    return status;
}

// Create and bind a socket given addrinfo results
int init_socket(struct addrinfo *results)
{
    int socketfd;
    int reuse_port = 1;

    // Results will point to a linked list of one or more addrinfo structures (p)
    // each of which contains a struct sockaddr (response information about the host)
    struct addrinfo *p;
    for (p = results; p != NULL; p = p->ai_next) {
        // Create a candidate socket
        socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (socketfd == -1) continue;

        // Check if address is already in use
        if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &reuse_port, sizeof reuse_port) == -1) {
            perror("setsockopt");
            close(socketfd);
            freeaddrinfo(results);
            return -1;
        }

        // Bind socket to address/port
        if (bind(socketfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(socketfd);
            continue;
        }

        // At this point, the socket file descriptor is set
        // Break the loop and return the file descriptor
        return socketfd;
    }

    return -1;
}

