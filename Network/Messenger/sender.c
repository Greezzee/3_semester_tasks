#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct sockaddr_in addr, addr_to_send;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(0);
    addr.sin_addr.s_addr = INADDR_ANY;
    bzero(addr.sin_zero, 8);
    struct sockaddr test;
    int my_socket;
    if ((my_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Unable to create socket\n");
        exit(-1);
    }

    if (bind(my_socket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Unable to bind\n");
        exit(-1);
    }
    addr_to_send = addr;
    addr_to_send.sin_port = htons(51707);
    inet_aton("10.0.2.15", &addr_to_send.sin_addr);
    printf("Binding success, sender started\n");
    for (int i = 0; i < 10; i++) {
        char mes[] = "Hello, world\n";
        sendto(my_socket, mes, sizeof(mes), 0, (struct sockaddr*)&addr_to_send, sizeof(addr_to_send));
        printf("Message sended\n");
    }
    printf("Sender exiting\n");
    return 0;
}