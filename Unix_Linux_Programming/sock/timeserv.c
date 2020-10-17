#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <strings.h>

#define PORTNUM 13000
#define HOSTLEN 256
#define oops(msg) {perror(msg); exit(1);}

int main(int argc, char *argv[])
{
	struct sockaddr_in saddr;
	struct hostent *hp;
	char hostname[HOSTLEN];
	int sockid, sockfd;
	FILE *sockfp;
	time_t thetime;
	sockid = socket(AF_INET, SOCK_STREAM, 0);
	if(sockid == -1) oops("socket");
	bzero((void *)&saddr, sizeof(saddr));
	gethostname(hostname, HOSTLEN);
	hp = gethostbyname(hostname);
	bcopy((void *)hp->h_addr, (void *)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(PORTNUM);
	saddr.sin_family = AF_INET;
	if(bind(sockid, (struct sockaddr *)&saddr, sizeof(saddr)) != 0)
		oops("bind");
	if(listen(sockid, SOMAXCONN) != 0)
		oops("listen");
	while(1){
		sockfd = accept(sockid, NULL, NULL);
		printf("got a call!\n");
		if(sockfd == -1) oops("accept");
		sockfp = fdopen(sockfd,"w");
		if(sockfp == NULL) oops("fdopen");
		thetime = time(NULL);
		fprintf(sockfp, "The time here is .. %s\n", ctime(&thetime));
		fclose(sockfp);
	}
}
