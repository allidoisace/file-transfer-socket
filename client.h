#ifndef client_h
#define client_h

// IO
#include <stdio.h>
// other
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
// data types for system calls
#include <sys/types.h>
// sockets
#include <sys/socket.h>
// internet domain addresses
#include <netinet/in.h>
// hostent
#include <netdb.h>

void error(char *msg);

#endif