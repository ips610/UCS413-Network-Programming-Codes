/*Required Headers*/
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// <sys/types.h> - system data types 
// <sys/socket.h> - internet protocol family -- socket() to create the sockets
// <netdb.h> - hostnet structure - official name of host and to access the db if it is stored
// <stdio.h> - basic input - output 
// <unistd.h> - fork(), close() system calls
// <string.h> - string


int main()
{
	char str[100];
	int listen_fd, comm_fd; // to handle the foriegn fd - comm_fd
	// fd - file descriptor
	// fd[0] - input stream
	// fd[1] - ouptut stream
	// fd[2] - error stream
	// fd[3] - socket descriptyer - socket number is retrned when socket is created -- generally the value is 3
	
	struct sockaddr_in servaddr; // structure 
	listen_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	// Zero (0) IS Default else IPPROTO_TCP, IPPROTO_UDP etc etc
	
	// Connection Type -> 1. Connectionless -> SOCK_DGRAM (2) -> UDP Protocol 2. Connetion -> SOCK_STREAM (1) -> TCP Protocol
	
	// AF_INET - to identify IP Protocol -> IPV4 --- 2 
	// AF_INET6 - IPV6 --- 10
	// listen_fd = 3 generally
	bzero( &servaddr, sizeof(servaddr));
	
	// bzero will fill all the zeros in the structure variable
	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	// htonl - host to network long - 
	// ntoh - router to host
	// INADDR_ANY - will accept data from any interface -> address: 0.0.0.0
	// aprart from this we can specify any ip address also to accept the data from that address only
	servaddr.sin_port = htons(22000);
	// htons - host to network short
	bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	listen(listen_fd, 10);
	// here 10 means that atleast 10 clients can connect with the server at the same type
	while(1)
		{
		
		comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
		bzero( str, 100);
		recv(comm_fd,str,100,0);
		printf("Echoing back - %s",str);
		send(comm_fd,str,strlen(str),0);
		close(comm_fd);

	}
}
