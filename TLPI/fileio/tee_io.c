#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define BUFMAX 4096

int main(int argc, char* argv[]){
	int fd;
	ssize_t numRead, numWritten;
	char buf[BUFMAX];
	
	fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | \
		S_IROTH | S_IWOTH);
	lseek(fd, 0, SEEK_SET);
	numRead = read(STDIN_FILENO, buf, BUFMAX);
	if(numRead == -1)
		return -1;
	if(numRead > 0){
		char out[numRead];
		strcpy(out, buf);
		write(STDOUT_FILENO, out, numRead);
		numWritten = write(fd, buf, numRead);
	}
	return 0;
}
