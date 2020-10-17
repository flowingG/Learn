/* pipedemo.c 	* Demonstrates : how to creat and use a pipe
 * 		* Effect : creates a pipe, writes into writing
 * 		  end, then runs around and reads from reading end.
 * 		  A little weird, buf demonstrates the idea.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

int main(){
	int len, i, pipefd[2];
	char buf[BUFSIZ];
	pipefd[0] = open("1.txt", O_CREAT | O_EXCL | O_RDWR);
	pipefd[1] = open("2.txt", O_CREAT | O_EXCL | O_RDWR);
	if(pipe(pipefd) == -1){
		perror("couldn't make pipe");
		exit(1);
	}
	printf("Got a pipe! It is file descriptors:{%d %d}\n", pipefd[0], pipefd[1]);
	while(fgets(buf, BUFSIZ, stdin)){
		len = strlen(buf);
		if(write(pipefd[1], buf, len) != len){
			perror("writing to pipe");
			break;
		}
		for(i = 0; i < len; ++i)
			buf[i] = 'X';
		len = read(pipefd[0], buf, BUFSIZ);
		if(len == -1){
			perror("reading from pipe");
			break;
		}
		if(write(STDOUT_FILENO, buf, len) != len){
			perror("writing to stdout");
			break;
		}
	}
	return 0;
}
