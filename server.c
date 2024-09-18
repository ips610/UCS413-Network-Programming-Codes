#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[100];
    socklen_t len;
    ssize_t n;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    bzero(&cliaddr, sizeof(cliaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;                // IPv4
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to any local address
    servaddr.sin_port = htons(22000);             // Port number

    // Bind the socket with the server address
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        return 1;
    }

    // Server loop
    while (1) {
        len = sizeof(cliaddr); // Length of client address
        bzero(buffer, sizeof(buffer));

        // Receive message from client
        n = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&cliaddr, &len);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        printf("Client: %s", buffer);

        // Echo back the message to the client
        n = sendto(sockfd, buffer, n, 0, (struct sockaddr *)&cliaddr, len);
        if (n < 0) {
            perror("sendto failed");
        }
    }

    close(sockfd);
    return 0;
}
