#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		perror("usage:command filename\n");
		return;
	}
	int fd = open(argv[1], O_WRONLY);
	lseek(fd, 100, SEEK_END);
	char buff[1024];
	int count = read(fd, buff, 1024);
	write(1, buff, count);
	lseek(fd, 20000, SEEK_END);
	write(fd, "hello", 6);
	
	return 0;
}
