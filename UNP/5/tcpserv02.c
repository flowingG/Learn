#include "unp.h"

void str_echo(int sockfd);
void sig_chld(int signo);

int main(int argc, char **argv)
{
	int listenfd, connfd;
	pid_t childpid;
	socklen_t clien;
	struct sockaddr_in cliaddr, servaddr;
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(24455);
	Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);
	Signal(SIGCHLD, sig_chld);
	for(;;)
	{
		clien = sizeof(cliaddr);
		connfd = Accept(listenfd, (SA *)&cliaddr, &clien);
		if((childpid = Fork()) == 0)
		{
			Close(listenfd);
			str_echo(connfd);
		}
		Close(connfd);
	}
}

void str_echo(int sockfd)
{
	ssize_t n;
	char buf[MAXLINE];
again:
	while((n = read(sockfd, buf, MAXLINE)) > 0)
		Writen(sockfd, buf, n);
	if(n < 0 && errno == EINTR)
		goto again;
	else if(n < 0)
		err_sys("str_echo:read error");
}

void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	pid = wait(&stat);
	printf("child %d terminated\n", pid);
	return;
}
