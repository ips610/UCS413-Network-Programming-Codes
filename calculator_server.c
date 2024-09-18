#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h> // Add this line for atof

int main() {
    char recvline[100], sendline[100];
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

    while (1) {
        client_len = sizeof(clientaddr);
        comm_fd = accept(listen_fd, (struct sockaddr *) &clientaddr, &client_len);

        // Step 1: Receive first number
        bzero(recvline, 100);
        recv(comm_fd, recvline, 100, 0);
        double num1 = atof(recvline);  // atof converts the string to a double
        printf("Received first number: %f\n", num1);

        // Step 2: Ask for operation
        strcpy(sendline, "Enter operation (+, -, *, /): ");
        send(comm_fd, sendline, strlen(sendline), 0);

        // Step 3: Receive operation
        bzero(recvline, 100);
        recv(comm_fd, recvline, 100, 0);
        char operation = recvline[0];
        printf("Received operation: %c\n", operation);

        // Step 4: Ask for second number
        strcpy(sendline, "Enter second number: ");
        send(comm_fd, sendline, strlen(sendline), 0);

        // Step 5: Receive second number
        bzero(recvline, 100);
        recv(comm_fd, recvline, 100, 0);
        double num2 = atof(recvline);
        printf("Received second number: %f\n", num2);

        // Step 6: Compute result
        double result = 0;
        switch (operation) {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/': 
                if (num2 != 0) 
                    result = num1 / num2; 
                else 
                    strcpy(sendline, "Error: Division by zero.\n");
                break;
            default:
                strcpy(sendline, "Error: Invalid operation.\n");
                send(comm_fd, sendline, strlen(sendline), 0);
                close(comm_fd);
                continue;
        }

        // Step 7: Send result
        if (operation != '/' || num2 != 0) {
            snprintf(sendline, 100, "%f\n", result);
        }
        send(comm_fd, sendline, strlen(sendline), 0);

        close(comm_fd);
    }

    close(listen_fd);
    return 0;
}
