#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#define MAX 1024

int main(int argc, char *argv[]){
	int fd;
	char buf[MAX];
	fd = open(argv[1], O_WRONLY | O_APPEND);
	lseek(fd, 0, SEEK_SET);	
	scanf("%s", buf);
	write(fd, buf, MAX);
	return 0;
}
