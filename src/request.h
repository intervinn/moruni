#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

struct request {
    char* method;
    char* route;
};

struct request request_new(int socket, int size) {
    char* req = (char*)malloc(size * sizeof(char));
    read(socket, req, size);

    char method[7];
    char route[100];
    
    sscanf(req, "%s %s", method, route);

    struct request r = {
        .method = strtok(method, ""),
        .route = strtok(route, "")
    };

    return r;
}