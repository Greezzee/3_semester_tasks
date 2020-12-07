#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

#define DATA_SIZE 16
#define IP_LEN 17

enum package_t {
    ERROR,
    INCORRECT_MES,
    CONNECTION_REQ,
    CONNECTION_ACCEPT,
    CONNECTION_DENY,

    DISCONNECT
};

struct network_package {
    enum package_t type;
    char data[DATA_SIZE];
};

struct sockaddr_in InitClient() {
    struct sockaddr_in client;
    client.sin_family = AF_INET;
    client.sin_port = INADDR_ANY;
    inet_aton("127.0.0.1", &client.sin_addr);
    return client;
}

struct sockaddr_in InitServer(char IP[IP_LEN], unsigned host) {
    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(host);
    inet_aton(IP, &server.sin_addr);
    return server;
}   

int CreateSocket(struct sockaddr_in* addr) {
    int my_socket;
    if ((my_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Unable to create socket\n");
        exit(-1);
    }

    if (bind(my_socket, (struct sockaddr*)addr, sizeof(*addr)) < 0) {
        perror("Unable to bind\n");
        exit(-1);
    }
    return my_socket;
}

void RecievePackageFromServer(int my_socket, struct sockaddr_in* server, struct network_package* package) {
    while (1) {
        struct sockaddr_in buf;
        socklen_t buf_len;
        recvfrom(my_socket, package, sizeof(struct network_package), 0, (struct sockaddr*)&buf, &buf_len);
        if (buf.sin_addr.s_addr == server->sin_addr.s_addr && buf.sin_port == server->sin_port)
            return;
        printf("Wrong server\n");
    }
}

int main() {
    struct sockaddr_in server = {}, client = {};

    char server_ip[IP_LEN] = {};
    unsigned server_port;

    printf("Enter IP to connect: ");
    scanf("%s", server_ip);
    printf("Enter port: ");
    scanf("%u", &server_port);

    client = InitClient();
    server = InitServer(server_ip, server_port);
    int client_socket = CreateSocket(&client);

    struct network_package pack;
    pack.type = CONNECTION_REQ;
    sendto(client_socket, &pack, sizeof(pack), 0, (struct sockaddr*)&server, sizeof(server));
    RecievePackageFromServer(client_socket, &server, &pack);
    if (pack.type != CONNECTION_ACCEPT) {
        printf("Server error\n");
        exit(-1);
    }
    printf("Connected to server\n");
    while(1);
}