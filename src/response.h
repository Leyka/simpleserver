#ifndef WEBSERVER_RESPONSE_H
#define WEBSERVER_RESPONSE_H

#define RESPONSE_BUFFER_SIZE 32000

typedef struct Response {
    int sockfd;
} Response;

void send_response(Response *res, char *header, char *content_type, void *body, size_t content_length);
void send_html(Response *res, char *body);
void send_404(Response *res);
Response* create_response(int sockfd);
void free_response(Response*);

#endif
