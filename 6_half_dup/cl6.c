#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int cs;
    struct sockaddr_in sa;
    char b[1024];
    cs = socket(AF_INET, SOCK_STREAM, 0);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);
    connect(cs, (struct sockaddr*)&sa, sizeof(sa));
    printf("Connection established. Start chatting!\n");
    while (1) {
        printf("Client: ");
        fgets(b, sizeof(b), stdin);
        send(cs, b, strlen(b), 0);
        if (strcmp(b, "bye\n") == 0) {
            printf("Terminating connection. Exiting...\n");
            break;
        }
        memset(b, 0, sizeof(b));
        recv(cs, b, 1024, 0);
        printf("Server: %s", b);
        if (strcmp(b, "bye\n") == 0) {
            printf("Server has disconnected. Exiting...\n");
            break;
        }
    }
    close(cs);
    return 0;
}
