#include "unp.h"

int main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_un servaddr;
	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sun_family = AF_LOCAL;
	sercpy(servaddr.sun_path, UNIXSTR_PATH);
	connect(sockfd, (SA *)&servaddr, sizeof(servaddr));
	str_cli(stdin, sockfd);
	return 0;
}
