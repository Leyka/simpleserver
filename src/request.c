#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include "request.h"

// Handle HTTP/1.1 Request
Request* create_request(int sockfd)
{
    char request[REQUEST_BUFFER_SIZE];
    recv(sockfd, request, REQUEST_BUFFER_SIZE, 0);

    Request *req = (Request*) malloc(sizeof(Request));

    // Analyze first request line
    printf("%s\n", request);
    sscanf(request, "%s %s %s", req->method, req->path, req->protocol);
    return req;
}

void clear_request(int sockfd, Request* req)
{
    free(req);
    close(sockfd);
}