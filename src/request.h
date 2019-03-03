#ifndef WEBSERVER_REQUEST_H
#define WEBSERVER_REQUEST_H

#define REQUEST_BUFFER_SIZE 2000 // bytes

typedef struct request
{
    char method[6]; // GET, POST, PUT, DELETE
    char path[64]; // /path/to/route
    char protocol[8]; // HTTP/1.1
} Request;

Request* create_request(int sockfd);
void clear_request(int sockfd, Request* req);

#endif
