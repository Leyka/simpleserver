#ifndef WEBSERVER_COMMON_TYPES_H
#define WEBSERVER_COMMON_TYPES_H

#include "request.h"
#include "response.h"

typedef void (*http_func_t)(Request*, Response*);

#endif
