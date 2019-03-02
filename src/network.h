#ifndef WEBSERVER_NETWORK_H
#define WEBSERVER_NETWORK_H

#define NB_CONNECTIONS_ALLOWED 5

int setup_addrinfo(char* port, struct addrinfo **results);
int init_socket(struct addrinfo *results);
int create_listener_socket(char* port);

#endif
