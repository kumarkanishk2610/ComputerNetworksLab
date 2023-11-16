//tcpipmsg cl
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int client_socket;
    struct sockaddr_in server_addr = {AF_INET, htons(8080), inet_addr("127.0.0.1")};
    char message[1024];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    printf("Enter a message: ");
    fgets(message, sizeof(message), stdin);
    send(client_socket, message, strlen(message), 0);

    close(client_socket);
    return 0;
}


//sv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr = {AF_INET, htons(8080), INADDR_ANY};
    socklen_t addr_size = sizeof(struct sockaddr_in);
    char buffer[1024];
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);
    client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &addr_size);
    recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[strcspn(buffer, "\n")] = '\0';
    printf("Received message from client: %s\n", buffer);
    close(client_socket);
    close(server_socket);
    return 0;
}


//echo udp cl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Connected to server\n");
    char message[1024];
    printf("Enter message to send to server: ");
    fgets(message, sizeof(message), stdin);
    send(clientSocket, message, strlen(message), 0);
    recv(clientSocket, message, sizeof(message), 0);
    printf("Received from server: %s\n", message);
    close(clientSocket);
    return 0;
}


//sv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(struct sockaddr_in);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    printf("Server listening on port %d...\n", 12345);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    printf("Connection accepted from %s:%d\n",
           inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Received from client: %s\n", buffer);
    send(clientSocket, buffer, strlen(buffer), 0);
    close(clientSocket);
    close(serverSocket);
    return 0;
}



//daytime cl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &(serverAddr.sin_addr));
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    recv(clientSocket, buffer, sizeof(buffer), 0);
    printf("Received date and time from server: %s\n", buffer);
    close(clientSocket);
    return 0;
}



//sv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <time.h>
void handleClient(int clientSocket) {
    time_t currentTime;
    struct tm* timeInfo;
    char timeString[1024];
    time(&currentTime);
    timeInfo = localtime(&currentTime);
    strftime(timeString, sizeof(timeString), "%Y-%m-%d %H:%M:%S", timeInfo);
    send(clientSocket, timeString, strlen(timeString), 0);
    close(clientSocket);
}
int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    printf("Server listening on port %d...\n", 8080);
    while (1) {
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (fork() == 0) {
            close(serverSocket);
            handleClient(clientSocket);
            exit(0);
        } else {
            close(clientSocket);
        }
    }
    return 0;
}

//halfdupcl
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


//sv
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


//fulldupcl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
int main() {
    int socketDescriptor;
    struct sockaddr_in serverAddress;
    char sendBuffer[8000], recvBuffer[8000];
    pid_t cpid;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(9652);
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    connect(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    cpid = fork();
    if (cpid == 0) {
        while (1) {
            bzero(&sendBuffer, sizeof(sendBuffer));
            printf("\nType a message here ... ");
            fgets(sendBuffer, sizeof(sendBuffer), stdin);
            send(socketDescriptor, sendBuffer, strlen(sendBuffer) + 1, 0);
            printf("\nMessage sent!\n");
        }
    } else {
        while (1) {
            bzero(&recvBuffer, sizeof(recvBuffer));
            recv(socketDescriptor, recvBuffer, sizeof(recvBuffer), 0);
            printf("\nSERVER: %s\n", recvBuffer);
        }
    }
    return 0;
}


//sv

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
int main(int argc, char *argv[]) {
    int clientSocketDescriptor, socketDescriptor;
    struct sockaddr_in serverAddress, clientAddress;
    socklen_t clientLength;
    char recvBuffer[8000], sendBuffer[8000];
    pid_t cpid;
    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9652);
    socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    bind(socketDescriptor, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(socketDescriptor, 5);
    printf("%s\n", "Server is running ...");
    clientSocketDescriptor = accept(socketDescriptor, (struct sockaddr*)&clientAddress, &clientLength);
    cpid = fork();
    if (cpid == 0) {
        while (1) {
            bzero(&recvBuffer, sizeof(recvBuffer));
            recv(clientSocketDescriptor, recvBuffer, sizeof(recvBuffer), 0);
            printf("\nCLIENT: %s\n", recvBuffer);
        }
    } else {
        while (1) {
            bzero(&sendBuffer, sizeof(sendBuffer));
            printf("\nType a message here ... ");
            fgets(sendBuffer, sizeof(sendBuffer), stdin);
            send(clientSocketDescriptor, sendBuffer, strlen(sendBuffer) + 1, 0);
            printf("\nMessage sent!\n");
        }
    }
    return 0;
}



//ftpcl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
void upload_file(int socket, const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    const char* upload_command = "UPLOAD";
    send(socket, upload_command, strlen(upload_command), 0);
    char buffer[1024];
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(socket, buffer, bytes_read, 0);
    }
    fclose(file);
    printf("File uploaded successfully.\n");
}
int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char buffer[1024];
    ssize_t bytes_received;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_port = htons(12345);
    connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    printf("Server: %s", buffer);
    const char* command = "Hello, server!\n";
    send(client_socket, command, strlen(command), 0);
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    buffer[bytes_received] = '\0';
    printf("Server Echo: %s", buffer);
    const char* filename = "example_file.txt";
    upload_file(client_socket, filename);
    close(client_socket);
    return 0;
}


//sv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
void handle_client(int client_socket) {
    char buffer[1024];
    ssize_t bytes_received;
    const char* welcome = "Welcome to the FTP server.\n";
    send(client_socket, welcome, strlen(welcome), 0);
    while (1) {
        bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) break;
        buffer[bytes_received] = '\0';
        if (strcmp(buffer, "UPLOAD") == 0) {
            FILE* file = fopen("uploaded_file.txt", "wb");
            while ((bytes_received = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
                fwrite(buffer, 1, bytes_received, file);
            }
            fclose(file);
            printf("File uploaded successfully.\n");
        } else {
            printf("Received: %s", buffer);
            send(client_socket, buffer, bytes_received, 0);
        }
    }
    close(client_socket);
}
int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(12345);
    bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
    listen(server_socket, 5);
    printf("FTP Server listening on port %d...\n", 12345s);
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        handle_client(client_socket);
    }
    close(server_socket);
    return 0;
}


//cmdcl

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    printf("Connected to the server.\n");
    const char* commandToSend = "ls";
    send(clientSocket, commandToSend, strlen(commandToSend), 0);
    close(clientSocket);
    return 0;
}



//sv
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    char command[1024];
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);
    printf("Server is listening for connections...\n");
    addrSize = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrSize);
    printf("Client connected.\n");
    memset(command, 0, sizeof(command));
    recv(clientSocket, command, sizeof(command), 0);
    printf("Command received from client: %s\n", command);
    system(command);
    close(clientSocket);
    close(serverSocket);
    return 0;
}

//arp

#include <sys/types.h>
#include <sys/socket.h>
#include <net/if_arp.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/if_ether.h>
#include <stdlib.h>
int main() {
    struct sockaddr_in sin = {0};
    struct arpreq myarp = {{0}};
    unsigned char *ptr;
    int sd;
    sin.sin_family = AF_INET;
    printf("Enter IP address: ");
    char ip[20];
    scanf("%s", ip);
    if (inet_pton(AF_INET, ip, &sin.sin_addr) == 0) {
        printf("IP address Entered '%s' is not valid \n", ip);
        exit(0);
    }
    memcpy(&myarp.arp_pa, &sin, sizeof(myarp.arp_pa));
    strcpy(myarp.arp_dev, "ens33");
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    printf("\nSend ARP request\n");
    if (ioctl(sd, SIOCGARP, &myarp) == -1) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }
    close(sd);
    ptr = &myarp.arp_ha.sa_data[0];
    printf("Received ARP Reply\n");
    printf("\nMAC Address for '%s' : ", ip);
    printf("%02X:%02X:%02X:%02X:%02X:%02X\n", ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);
    return 0;
}

