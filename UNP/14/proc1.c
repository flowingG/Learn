#include <stdio.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

int main()
{
	char *str1 = "hello ";
	char *str2 = "world.\n";
	struct iovec iov[2];
	iov[0].iov_base = str1;
	iov[1].iov_base = str2;
	iov[0].iov_len = strlen(str1);
	iov[1].iov_len = strlen(str2);
	int nwriten;
	while(1)
		nwriten = writev(fileno(stdout), iov, 2);
	return 0;
}
