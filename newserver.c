#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>  // For inet_ntop

int main() {
    char str[100];
    int listen_fd, comm_fd;
    struct sockaddr_in servaddr, clientaddr;
    socklen_t client_len;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(22000);

    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listen_fd, 10);

    while(1) {
        client_len = sizeof(clientaddr);
        comm_fd = accept(listen_fd, (struct sockaddr*) &clientaddr, &client_len);

        // Print the client's IP address
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientaddr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Connected to client IP: %s\n", client_ip);

        while(1) {
            bzero(str, 100);
            int n = recv(comm_fd, str, 100, 0);
            if(n <= 0) {
                printf("Client disconnected or error occurred\n");
                break;
            }
            printf("Echoing back - %s", str);
            send(comm_fd, str, strlen(str), 0);
        }
        close(comm_fd);
    }
}
