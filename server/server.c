// file transfer using IPv4
// allidoisace (aka developerace)

#include "server.h"

int main(int argc, char **argv) {
	// *fd are file descriptors, store return values from socket and accept system call
	// portno is port number
	// clientlen is size of address for client, needed for accept()
	int sockfd, newsockfd, portno, clientlen;
	

	// IPv4 version
	// serv_addr stores server address
	// cli_addr stores client address
	struct sockaddr_in serv_addr, cli_addr;
	
	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided.\n");
		exit(1);
	}

	// initialize socket
	// socket(domain, type of socket, protocol)
	// if protocol is 0, OS will choose appropriate, however TCP for Streams, UDP for Datagrams
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket.\n");
	}

	memset((char *)&serv_addr, 0, sizeof(serv_addr));
	// bzero( ,  );
	portno = atoi(argv[1]);

	// IPv4 version
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htons(INADDR_ANY);
	serv_addr.sin_port = htons(portno);

	// bind socket
	if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding.\n");
	}

	// listen (backlog of 5)
	listen(sockfd, 5);

	// accept
	clientlen = sizeof(cli_addr);
	pthread_t thread_id;
	while( newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clientlen) ) {
		printf("CONNECTION ACCEPTED.\n");
		if( pthread_create(&thread_id, NULL, transferFile, (void*) &newsockfd) != 0) {
			perror("Thread could not be created.");
			return 1;
		}
	}


	if (newsockfd < 0) {
		error("ERROR on accept.\n");
	}

	// memset(file_name, 0, sizeof(file_name));
	// n = read(newsockfd, file_name, 255);
	// if (n < 0) {
	// 	error("ERROR reading from socket.\n");
	// }

	// FILE *fp;
	// int ch = 0;
	// fp = fopen(file_name, "ab");
	// if (NULL == fp) {
	// 	printf("ERROR opening file.\n");
	// 	return 1;
	// }

	// int bytesReceived = 0;
	// memset(buffer, 0, sizeof(buffer));
	// while((bytesReceived = read(newsockfd, buffer, 1024)) > 0) {
	// 	fflush(stdout);
	// 	fwrite(buffer, 1,bytesReceived,fp);
	// }

	// printf("The file has been received.\n");

	// // close client socket fd
	// if (close(newsockfd) < 0) {
	// 	printf("ERROR closing newsockfd\n");
	// }
	// close server socket fd
	if (close(sockfd) < 0) {
		printf("ERROR closing sockfd\n");
	}

	return 0;
}

void *transferFile(void *socket) {
	int sock = *(int*)socket;
	char buffer[1024];
	char file_name[256];
	int n;

	memset(file_name, 0, sizeof(file_name));
	n = read(sock, file_name, 255);
	if (n < 0) {
		error("ERROR reading from socket.\n");
	}

	FILE *fp;
	int ch = 0;
	fp = fopen(file_name, "ab");
	if (NULL == fp) {
		printf("ERROR opening file.\n");
		exit(1);
	}

	int bytesReceived = 0;
	memset(buffer, 0, sizeof(buffer));
	while((bytesReceived = read(sock, buffer, 1024)) > 0) {
		fflush(stdout);
		fwrite(buffer, 1, bytesReceived, fp);
	}
	fclose(fp);

	printf("The file has been received.\n");

	// close client socket fd
	if (close(sock) < 0) {
		printf("ERROR closing socket\n");
	}
}

void error(char *msg) {
	perror(msg);
	exit(1);
}