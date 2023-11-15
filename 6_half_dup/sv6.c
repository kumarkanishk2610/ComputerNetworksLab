#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int ss, cs;
    struct sockaddr_in sa, ca;
    char b[1024];
    ss = socket(AF_INET, SOCK_STREAM, 0);
    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = INADDR_ANY;
    sa.sin_port = htons(8080);
    bind(ss, (struct sockaddr*)&sa, sizeof(sa));
    listen(ss, 1);
    socklen_t cl = sizeof(ca);
    cs = accept(ss, (struct sockaddr*)&ca, &cl);
    printf("Connection established. Start chatting!\n");
    while (1) {
        memset(b, 0, sizeof(b));
        recv(cs, b, 1024, 0);
        printf("Client: %s", b);
        if (strcmp(b, "bye\n") == 0) break;
        memset(b, 0, sizeof(b));
        printf("Server: ");
        fgets(b, sizeof(b), stdin);
        send(cs, b, strlen(b), 0);
        if (strcmp(b, "bye\n") == 0) break;
    }
    close(cs);
    close(ss);
    return 0;
}
