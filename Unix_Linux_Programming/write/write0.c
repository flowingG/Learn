/* write0.c
 *
 * purpose:send message to another terminal
 * method:open the other terminal for output then
 * 	copy from stdin to that terminal
 * shows:a terminal is just a file supporting regular i/o
 * usage:write0 ttyname
 */
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
	int fd;
	char buf[BUFSIZ];
	if(argc != 2){
	fprintf(stderr, "usage:write0 ttyname\n");
	exit(1);
	}
	fd = open(argv[1], O_WRONLY);
	if(fd == -1){
		perror(argv[1]);
		exit(1);
	}
	while(fgets(buf, BUFSIZ, stdin) != NULL)
		if(write(fd, buf, strlen(buf)) == -1)
			break;
	close(fd);
	return 0;
}
