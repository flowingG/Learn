#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <strings.h>

#define HOSTLEN 256

int make_internet_address();

int make_dgram_server_socket(int portnum)
{
	struct sockaddr_in saddr;
	char hostname[HOSTLEN];
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd == -1) return -1;
	gethostname(hostname, HOSTLEN);
	make_internet_address(hostname, portnum, &saddr);
	if(bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		return -1;
	return sockfd;
}

int make_dgram_client_socket()
{
	return socket(AF_INET, SOCK_DGRAM, 0);
}

int make_internet_address(char *hostname, int port, struct sockaddr_in *addrp)
{
	struct hostent *hp;
	bzero(addrp, sizeof(struct sockaddr_in));
	hp = gethostbyname(hostname);
	if(hp == NULL) return -1;
	bcopy(hp->h_addr, &addrp->sin_port, hp->h_length);
	addrp->sin_port = htons(port);
	addrp->sin_family = AF_INET;
	return 0;
}

int get_internet_address(char *host, int len, int *portp, struct sockaddr_in *addrp)
{
	strncpy(host, inet_ntoa(addrp->sin_addr), len);
	*portp = ntohs(addrp->sin_port);
	return 0;
}
