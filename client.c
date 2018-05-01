// file transfer using IPv4
// allidoisace (aka developerace)

#include "client.h"

int main(int argc, char *argv[]) {
    int sockfd, portno, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    char file_name[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // bcopy copys string 1 to string 2 based on the length.
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR connecting");
    }

    // printf("Please enter the message: ");

    // bzero(buffer,1024);
    // fgets(buffer,1023,stdin);
    // n = write(sockfd,buffer,strlen(buffer));
    // if (n < 0) {
    //     error("ERROR writing to socket");
    // }

    // bzero(buffer,1024);
    // n = read(sockfd,buffer,1023);
    // if (n < 0) {
    //     error("ERROR reading from socket");
    // }
    // printf("%s\n",buffer);


    printf("Please enter the file name: ");
    bzero(file_name,256);
    fgets(file_name,255,stdin);
    n = write(sockfd,file_name,strlen(file_name));
    if (n < 0) {
        error("ERROR writing to socket");
    }

    bzero(buffer, 1024);
    FILE *f;
    int words = 0;
    char c;
    f = fopen("new.txt", "r");

    if(f == NULL) {
        perror ("Error opening file");
    } else {
        while(1) {
            int nread = fread(buffer,1,1024,f);
            printf("Bytes read %d \n", nread);

            if(nread > 0) {
                write(sockfd, buffer, nread);
            }
            if (nread < 1024) {
                if (feof(f)) {
                    printf("End of file\n");
                }
                if (ferror(f)) {
                    printf("Error reading\n");
                }
                break;
            }
        }
    }

    printf("This file has been successfully sent.\n");



    shutdown(sockfd, 2);    

    return 0;
}

void error(char *msg) {
    perror(msg);
    exit(0);
}