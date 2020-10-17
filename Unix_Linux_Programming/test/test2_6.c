#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		perror("usage:command filename\n");
		return 1;
	}
	int fd1, fd2, fd3;
	fd1 = open(argv[1], O_RDONLY);
	fd2 = open(argv[1], O_WRONLY);
	fd3 = open(argv[1], O_RDONLY);
	char buff[20];
	int count = read(fd1, buff, 20);
	write(1, buff, count);
	write(fd2, "testing 123...", 15);
	count = read(fd3, buff, 20);
	write(1, buff, count);
	printf("\n");
	return 0;
}
