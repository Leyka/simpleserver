#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include "response.h"

static char* datetime_now()
{
    time_t rawtime;
    time(&rawtime);
    struct tm *timeinfo = localtime(&rawtime);
    return asctime(timeinfo);
}

void send_response(Response *res, char *header, char *content_type, void *body, size_t content_length)
{
    char response[RESPONSE_BUFFER_SIZE];

    if (header == NULL) {
        header = "HTTP/1.1 200 OK";
    } else if (content_type == NULL) {
        content_type = "text/html; charset=utf-8";
    }

    size_t length = sprintf(response,
                    "%s\n"
                    "Connection: close\n"
                    "Content-Type: %s\n"
                    "Content-Length: %d\n"
                    "Date: %s\n\n"
                    "%s\n",
                    header, content_type, content_length, datetime_now(), body);

    // Send response via socket
    ssize_t status = send(res->sockfd, response, length, 0);
    if (status == 0) {
        perror("send response");
    }
}

void send_html(Response *res, char *body)
{
    send_response(res, NULL, NULL, body, strlen(body));
}

void send_404(Response *res)
{
    char *header = "HTTP/1.1 404 NOT FOUND";
    char *content_type = "text/plain";
    char *body = "404 Page Not Found";
    size_t length = strlen(body);

    send_response(res, header, content_type, body, length);
}

Response* create_response(int sockfd)
{
    Response *res = (Response*) malloc(sizeof(Response));
    res->sockfd = sockfd;
    return res;
}

void free_response(Response* res)
{
    free(res);
}