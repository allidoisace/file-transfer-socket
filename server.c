// file transfer using IPv4
// allidoisace (aka developerace)

#include "server.h"

int main(int argc, char **argv) {
	// *fd are file descriptors, store return values from socket and accept system call
	// portno is port number
	// clientlen is size of address for client, needed for accept()
	int sockfd, newsockfd, portno, clientlen;
	char buffer[1024];
	char file_name[256];

	// IPv4 version
	// serv_addr stores server address
	// cli_addr stores client address
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided.\n");
		exit(1);
	}

	// initialize socket
	// socket(domain, type of socket, protocol)
	// if protocol is 0, OS will choose appropriate, however TCP for Streams, UDP for Datagrams
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket.");
	}

	bzero( (char *)&serv_addr, sizeof(serv_addr) );
	portno = atoi(argv[1]);

	// IPv4 version
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr.sin_port = htons(portno);

	// bind socket
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding.");
	}

	// listen (backlog of 5)
	listen(sockfd, 5);

	// accept
	clientlen = sizeof(cli_addr);
	newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clientlen);
	if (newsockfd < 0) {
		error("ERROR on accept.");
	}

	bzero(file_name, 256);
	n = read(newsockfd, file_name, 255);
	if (n < 0) {
		error("ERROR reading from socket.");
	}

	printf("%s\n", file_name);

	FILE *fp;
	int ch = 0;
	fp = fopen("test.txt", "a");
	if (NULL == fp) {
		printf("Error opening file");
		return 1;
	}

	int bytesReceived = 0;
	bzero(buffer, 1024);
	while((bytesReceived = read(newsockfd, buffer, 1024)) > 0) {
		fflush(stdout);
		fwrite(buffer, 1,bytesReceived,fp);
	}

	printf("The file has been received.\n");

	/*
	// read from socket
	bzero(buffer, 1024);
	n = read(newsockfd, buffer, 1023);
	if (n < 0) {
		error("ERROR reading from socket.");
	}

	
	// write back into socket
	n = write(newsockfd, "I got your message.", 19);
	if (n < 0) {
		error("ERROR writing to socket.");
	}
	*/

	// close socket fd's
	shutdown(sockfd, 2);
	shutdown(newsockfd, 2);

	return 0;
}

void error(char *msg) {
	perror(msg);
	exit(1);
}