#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 22000
#define BUF_SIZE 100

void *receive_messages(void *sockfd_ptr) {
    int sockfd = *(int *)sockfd_ptr;
    struct sockaddr_in clientaddr;
    socklen_t client_len = sizeof(clientaddr);
    char recvline[BUF_SIZE];

    while(1) {
        bzero(recvline, BUF_SIZE);
        int n = recvfrom(sockfd, recvline, BUF_SIZE, 0, (struct sockaddr *) &clientaddr, &client_len);
        if(n <= 0) {
            printf("Error occurred or client disconnected\n");
            break;
        }
        printf("Client: %s", recvline);
    }
    return NULL;
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    pthread_t recv_thread;
    char sendline[BUF_SIZE];
    struct sockaddr_in clientaddr;
    socklen_t client_len = sizeof(clientaddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    pthread_create(&recv_thread, NULL, receive_messages, &sockfd);

    while(1) {
        bzero(sendline, BUF_SIZE);
        fgets(sendline, BUF_SIZE, stdin);
        if (sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *) &clientaddr, client_len) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(sockfd);
    return 0;
}
