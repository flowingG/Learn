#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dgram.c"

#define oops(m, x) {perror(m); exit(x); }

int main(int argc, char *argv[])
{
	int sockfd;
	char *msg;
	struct sockaddr_in saddr;
	if(argc != 4)
		oops("usage:dgsend host port 'message'\n",1);
	msg = "testing 1234";
	if((sockfd = make_dgram_client_socket()) == -1)
		oops("can't make socket", 3);
	make_internet_address(argv[1], atoi(argv[2]), &saddr);
	if(sendto(sockfd, msg, strlen(msg) + 1, 0, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
		oops("sendto failed", 3);
	return 0;
}
