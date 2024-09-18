#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int sockfd;
    char sendline[100], recvline[100];
    struct sockaddr_in servaddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(22000);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        return 1;
    }

    // Step 1: Send first number
    printf("Enter the first number: ");
    fgets(sendline, 100, stdin);
    send(sockfd, sendline, strlen(sendline), 0);

    // Step 2: Receive prompt for operation
    bzero(recvline, 100);
    recv(sockfd, recvline, 100, 0);
    printf("Server: %s", recvline);

    // Step 3: Send operation
    fgets(sendline, 100, stdin);
    send(sockfd, sendline, strlen(sendline), 0);

    // Step 4: Receive prompt for second number
    bzero(recvline, 100);
    recv(sockfd, recvline, 100, 0);
    printf("Server: %s", recvline);

    // Step 5: Send second number
    fgets(sendline, 100, stdin);
    send(sockfd, sendline, strlen(sendline), 0);

    // Step 6: Receive result
    bzero(recvline, 100);
    recv(sockfd, recvline, 100, 0);
    printf("Server: The result is %s", recvline);

    close(sockfd);
    return 0;
}
