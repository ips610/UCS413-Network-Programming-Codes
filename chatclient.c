#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

void *receive_messages(void *sockfd_ptr) {
    int sockfd = *(int *)sockfd_ptr;
    char recvline[100];
    
    while(1) {
        bzero(recvline, 100);
        int n = recv(sockfd, recvline, 100, 0);
        if(n <= 0) {
            printf("Server disconnected or error occurred\n");
            break;
        }
        printf("Server: %s", recvline);
    }
    return NULL;
}

int main(int argc, char **argv) {
    int sockfd;
    char sendline[100];
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

    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_messages, &sockfd);

    while(1) {
        bzero(sendline, 100);
        fgets(sendline, 100, stdin);
        if (send(sockfd, sendline, strlen(sendline), 0) < 0) {
            perror("Send failed");
            break;
        }
    }

    close(sockfd);
    return 0;
}
