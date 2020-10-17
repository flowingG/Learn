#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>

int mydup(int oldfd);
int mydup2(int oldfd, int newfd);

int main(int argc, char *argv[]){
	int fd, newfd;
	if(argc != 2){
		printf("参数不对\n");
		return 0;
	}
	fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
	printf("描述符为%d\n", fd);
	printf("mydup操作后的描述符:");
	newfd = mydup(fd);
	printf("%d   %d\n", fd, newfd);
	newfd = 1000;
	printf("mydup2操作后的描述符:");
	mydup2(fd, newfd);
	printf("%d   %d\n", fd, newfd);
	return 0;
}

int mydup(int oldfd){
	if(fcntl(oldfd, F_GETFL) == -1){
		printf("打开的文件描述符不存在\n");
		errno = EBADF;
		return -1;
	}
	int newfd;
	newfd = fcntl(oldfd, F_DUPFD, 0);
	return newfd;
}

int mydup2(int oldfd, int newfd){
	if(fcntl(oldfd, F_GETFL) == -1){
		printf("打开的文件描述符不存在\n");
		errno = EBADF;
		return -1;
	}
	if(oldfd == newfd)
		return oldfd;
	if(fcntl(newfd, F_GETFL) != -1)
		close(newfd);
	return fcntl(oldfd, F_DUPFD, newfd);
}
