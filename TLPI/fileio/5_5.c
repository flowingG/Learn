#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int fd, nfd, flags1, flags2;
	off_t offset1, offset2;
	if(argc != 2){
		printf("参数不匹配\n");
		return -1;
	}
	fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	nfd = dup(fd);
	flags1 = fcntl(fd, F_GETFL);
	flags2 = fcntl(nfd, F_GETFL);
	if(flags1 == flags2)
		printf("文件状态标志相同\n");
	offset1 = lseek(fd, 0, SEEK_CUR);
	offset2 = lseek(nfd, 0, SEEK_CUR);
	if(offset1 == offset2)
		printf("文件偏移量相同\n");
	return 0;	
}
