#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>

#include "./request.h"
#include "./response.h"

struct server {
    int port;
    int conmax;
    int bufmax;
    int ni_maxhost;
    int ni_maxservice;

    struct sockaddr_in server_address;
    int server_socket;
    void(*on_request)(struct server*, int, struct request);
};

struct server server_new(int port, int conmax) {

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
        perror("server not bound to address");
        exit(1);
    }

    struct server s = {
        .port = port,
        .conmax = conmax,
        .bufmax = 1024,
        .ni_maxhost = 1025,
        .ni_maxservice = 32,

        .server_address = server_address,
        .server_socket = server_socket,
        .on_request = NULL
    };

    return s;
}

void server_bind(struct server* s, void(*on_request)(struct server*, int, struct request)) {
    s->on_request = on_request;
}

void server_respond(int sock, char* response) {
    send(sock, response, strlen(response), 0);
    free(response);
}

void server_run(struct server* s) {

    if (listen(s->server_socket, s->conmax) < 0) {
        printf("server not listening \n");
        exit(1);
    }

    char host_buf[NI_MAXHOST];
    char service_buf[NI_MAXSERV];

    int error = getnameinfo( (struct sockaddr*)&s->server_address, sizeof(s->server_address),
    host_buf, sizeof(host_buf), service_buf, sizeof(service_buf), 0);

    if (error != 0) {
        printf("error: %s\n", gai_strerror(error));
        exit(1);
    }

    printf("server is listening on http://%s:%s\n\n", host_buf, service_buf);

    char buf[s->conmax];

    while (1) {
        memset(buf, 0, s->conmax);
        int n = sizeof(buf);
        int client_sock = accept(s->server_socket, NULL, NULL);

        if (client_sock < 0) {
            perror("accept failed");
            continue;
        }

        struct request req = request_new(client_sock, s->bufmax);
        printf("%s-%s\n", req.method, req.route);
        s->on_request(s, client_sock, req);
        close(client_sock);
    }
}