#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dgram.c"

#define oops(m, x) {perror(m); exit(x);}

void say_who_called(struct sockaddr_in *);

int main(int argc, char *argv[])
{
	int port, sockfd;
	char buf[BUFSIZ];
	ssize_t msglen;
	struct sockaddr_in saddr;
	socklen_t saddrlen;
	if(argc == 1 || (port = atoi(argv[1])) <= 0)
		oops("usage:dgrecv portnumber\n", 1);
	if((sockfd = make_dgram_server_socket(port)) == -1)
		oops("can't make socket", 2);
	saddrlen = sizeof(saddr);
	while((msglen = recvfrom(sockfd, buf, BUFSIZ, 0, (struct sockaddr *)&saddr, &saddrlen)) > 0)
	{
		buf[msglen] = '\0';
		printf("dgrecv:got a message: %s\n", buf);
		say_who_called(&saddr);
	}
	return 0;
}

void say_who_called(struct sockaddr_in *saddr)
{
	char host[BUFSIZ];
	int port;
	get_internet_address(host, BUFSIZ, &port, saddr);
	printf("from %s : %d\n", host, port);
}
