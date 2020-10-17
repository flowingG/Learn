#include "unp.h"

int main(int argc, char **argv)
{
	int sockfd;
	socklen_t len;
	struct sockaddr_un addr1, addr2;
	if(argc != 2)
		err_quit("usage: unixbind <pathname>");
	sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
	unlink(argv[1]);
	bzero(&addr1, sizeof(addr1));
	addr1.sun_family = AF_LOCAL;
	strncpy(addr1.sun_path, argv[1], sizeof(addr1.sun_path) - 1);
	bind(sockfd, (SA *)&addr1, SUN_LEN(&addr1));
	len = sizeof(addr2);
	getsockname(sockfd, (SA *)&addr2, &len);
	printf("bound name = %s, return len = %d\n", addr2.sun_path, len);
	return 0;
}