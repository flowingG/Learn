#include "unp.h"

static void sig_alrm(int);

void dg_cli(FILE *fp, int sockfd, const SA *pservaddr, socklen_t servlen)
{
	int n;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	signal(SIGALRM, sig_alrm);
	while(fgets(sendline, MAXLINE, fp) != NULL)
	{
		sendto(sockfd, sendline, strlen(sendline), 0, pservaddr, servlen);
		alarm(5);
		if((n = recvfrom(sockfd, recvline, MAXLINE, 0, NULL, NULL)) < 0)
		{
			if(errno == EINTR)
				fprintf(stderr, "socket timeout\n");
			else 
				err_sys("recvfrom error");
		}
		else
		{
			alarm(0);
			recvline[n] = 0;
			fputs(recvline, stdout);
		}
	}
}

static void sig_alrm(int signo){ return; }
