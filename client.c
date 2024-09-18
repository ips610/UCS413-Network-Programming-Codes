#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char sendline[100];
    char recvline[100];
    socklen_t len;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));

    // Fill server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    len = sizeof(servaddr);

    // Client loop
    while (1) {
        bzero(sendline, sizeof(sendline));
        bzero(recvline, sizeof(recvline));

        // Get input from user
        printf("Enter message: ");
        fgets(sendline, sizeof(sendline), stdin);

        // Send message to server
        ssize_t n = sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *)&servaddr, len);
        if (n < 0) {
            perror("sendto failed");
            continue;
        }

        // Receive echoed message from server
        n = recvfrom(sockfd, recvline, sizeof(recvline), 0, NULL, NULL);
        if (n < 0) {
            perror("recvfrom failed");
            continue;
        }

        printf("Echo from server: %s", recvline);
    }

    close(sockfd);
    return 0;
}
