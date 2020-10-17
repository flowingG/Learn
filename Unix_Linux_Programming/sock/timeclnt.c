#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <unistd.h>

#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	struct hostent *hp;
	int connfd;
	char message[BUFSIZ];
	int messlen;
	if((connfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) oops("socket");
	bzero(&servaddr, sizeof(servaddr));
	if((hp = gethostbyname(argv[1])) == NULL) oops(argv[1]);
	bcopy(hp->h_addr, (struct sockaddr*)&servaddr.sin_addr, hp->h_length);
	servaddr.sin_port = htons(atoi(argv[2]));
	servaddr.sin_family = AF_INET;
	if(connect(connfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) oops("connect");
	if((messlen = read(connfd, message, BUFSIZ)) == -1) oops("read");
	if(write(STDOUT_FILENO, message, messlen) != messlen) oops("write");
	close(connfd);
}
