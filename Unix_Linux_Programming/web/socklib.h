#ifndef __SOCK_LIB_H_
#define __SOCK_LIB_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <sys/types.h>

#define HOSTLEN 256
#define BACKLOG 12

int make_server_socket_q(int, int);
int make_server_socket(int);
int connect_to_server(char *, int);

#endif
