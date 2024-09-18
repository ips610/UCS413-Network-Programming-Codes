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
    struct sockaddr_in servaddr;
    socklen_t serv_len = sizeof(servaddr);
    char recvline[BUF_SIZE];

    while(1) {
        bzero(recvline, BUF_SIZE);
        int n = recvfrom(sockfd, recvline, BUF_SIZE, 0, (struct sockaddr *) &servaddr, &serv_len);
        if(n <= 0) {
            printf("Error occurred or server disconnected\n");
            break;
        }
        printf("Server: %s", recvline);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int sockfd;
    struct sockaddr_in servaddr;
    pthread_t recv_thread;
    char sendline[BUF_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    pthread_create(&recv_thread, NULL, receive_messages, &sockfd);

    while(1) {
        bzero(sendline, BUF_SIZE);
        fgets(sendline, BUF_SIZE, stdin);
        if (sendto(sockfd, sendline, strlen(sendline), 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(sockfd);
    return 0;
}
