
#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "./jsmn.h"

struct response {
    char* status; // status string with message (like "200 OK")
    char* content_type; 
    char* value; // the body
    int reslen; // initialised after response_str
    jsmn_parser parser;
};

struct response response_new(char* status, char* content_type, char* value) {
    struct response r = {
        .content_type = content_type,
        .status = status,
        .value = value
    };
    return r;
}

char* response_str(struct response* r) {

    int reslen = strlen(r->content_type) + strlen(r->status) + strlen(r->value);

    char* response = (char*)malloc(reslen * sizeof(char));

    sprintf(response, "HTTP/1.1 %s\nContent-Type: %s\nContent-Length: %d\n\n%s",
        r->status, r->content_type, strlen(r->value), r->value
    );
    
    return response;
}


