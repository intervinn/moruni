#include "./server.h"

void on_request(struct server* s, int sock, struct request res) {
    printf("ffs the route is %s\n", res.route);
    if (strcmp(res.route, "")) {
        struct response r = response_new("200 OK", "text/plain", "welcome to /");
        server_respond(sock, response_str(&r));
        return;
    }

    struct response r = response_new("200 OK", "text/plain", "some wrong route m2");
    server_respond(sock, response_str(&r));
    return;
}   

int main() {
    struct server server = server_new(8808, 128);
    server_bind(&server, on_request);
    server_run(&server);
}
