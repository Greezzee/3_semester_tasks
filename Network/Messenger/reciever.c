#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct sockaddr_in addr, addr_to_recv;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(51707);
    inet_aton("10.0.2.15", &addr.sin_addr);
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

    printf("Binding success, reciever started\n");
    for (int i = 0; i < 10; i++) {
        char mes[14];
        socklen_t addr_size;
        recvfrom(my_socket, mes, 14, 0, (struct sockaddr*)&addr_to_recv, &addr_size);
        printf("Recieved message from %s:%hu: %s", inet_ntoa(addr_to_recv.sin_addr), ntohs(addr_to_recv.sin_port), mes);
    }
    printf("Sender exiting\n");
    return 0;
}