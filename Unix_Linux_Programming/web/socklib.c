#include "socklib.h"

int make_server_socket(int portnum){
	return make_server_socket_q(portnum, BACKLOG);
}

int make_server_socket_q(int portnum, int backlog){
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sockfd;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		return -1;
	bzero(&saddr, sizeof(saddr));
	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);
	bcopy(hp->h_addr, (struct sockaddr *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(portnum);
	saddr.sin_family = AF_INET;
	if(bind(sockfd, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		return -1;	
	if(listen(sockfd, backlog) != 0)
		return -1;
	return sockfd;
}

int connect_to_server(char *host, int portnum)
{
	int sockfd;
	struct sockaddr_in servaddr;
	struct hostent *hp;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd == -1)
		return -1;
	bzero(&servaddr, sizeof(servaddr));
	hp = gethostbyname(host);
	bcopy(hp->h_addr, (struct sockaddr *)&servaddr.sin_addr, hp->h_length);
	servaddr.sin_port = htons(portnum);
	servaddr.sin_family = AF_INET;
	if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
		return -1;
	return sockfd;
}
