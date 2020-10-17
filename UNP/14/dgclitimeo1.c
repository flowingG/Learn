#include "unp.h"

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	while(fgets(sendline, MAXLINE, fp) != NULL)
	{
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		if(readdable_timeo(sockfd, 5) == 0)
			fprintf(stderr, "socket timeout\n");
		else{
			n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL);
			recvline[n] = 0;
			fputs(recvline, stdout);
		}
	}
}
