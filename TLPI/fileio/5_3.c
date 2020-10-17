#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int fd;
	char c[1];
	int sum = atoi(argv[2]);
	if(argc == 3){
		fd = open(argv[1], O_APPEND | O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		printf("输入了三个参数\n");
		for(int i = 0; i < sum; i++){
			if(write(fd, c, 1) != 1)
				return 0;
		}
	}
	else if(argc == 4){
		fd = open(argv[1], O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
		for(int i = 0; i < sum; i++){
			lseek(fd, 0, SEEK_END);
			if(write(fd, c, 1) != 1)
				return 0;
		}
		printf("输入了四个参赛\n");
	}
	else{
		printf("命令行参数不为4或3\n");
		return 0;
	}
}
