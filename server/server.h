#ifndef server_h
#define server_h

// IO
#include <stdio.h>
// other
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
// data types for system calls
#include <sys/types.h>
// sockets
#include <sys/socket.h>
// internet domain addresses
#include <netinet/in.h>
// threading
#include<pthread.h>

void *transferFile(void *socket);
void error(char *msg);

#endif