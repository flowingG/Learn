/** cp1.c
 * version 1 of cp - uses read and open with tunable buffer size
 *
 * usage:cp1 src dest
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFERSIZE 4096
#define COPYMORE 0644

void oops(char *, char *);

int main(int argc, char **argv)
{
	int infd, outfd, count;
	char buf[BUFFERSIZE];
	if(argc != 3)
	{
		fprintf(stderr, "usage: %s source destination\n", *argv);
		exit(1);
	}
	if(strcmp(argv[1], argv[2]) == 0)
	{
		fprintf(stderr, "cp: '%s' and '%s' are the same file\n", argv[1], argv[2]);
		exit(1);
	}
	if((infd = open(argv[1], O_RDONLY)) == -1)
		oops("Can't open ", argv[1]);
	if((outfd = open(argv[2], O_WRONLY | O_CREAT)) == -1)
		oops("Can't creat ", argv[2]);
	while((count = read(infd, buf, BUFFERSIZE)) > 0)
		if(write(outfd, buf, count) != count)
			oops("Write error to ", argv[2]);
	if(count == -1)
		oops("Read error from ", argv[1]);
	if(close(infd) == -1 || close(outfd) == -1)
		oops("Error closing file", "");
	return 0;
}

void oops(char *s1, char *s2)
{
	fprintf(stderr, "Error: %s", s1);
	perror(s2);
	exit(1);
}
