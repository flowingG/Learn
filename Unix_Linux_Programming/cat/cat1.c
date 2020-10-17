/** cat1.c
 *  version 1 of cat - uses read, write and open with tunable buffer size
 *
 *  usage : cat
 *  	  - cat filename
 *  	  - cat argument filename 
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define MAXSIZE 1024

int main(int argc, char **argv)
{
	if(argc != 1 && argc != 2)
	{
		perror("usage : cat or cat filename\n");
		return 1;
	}
	int inputfd;
	if(argc == 1)
	{
		inputfd = 0;
	}
	if(argc == 2)
	{
		inputfd = open(argv[1], O_RDONLY);
	}
	int count = 0;
	char buff[MAXSIZE];
	while((count = read(inputfd, buff, MAXSIZE)) > 0)
		write(1, buff, count);
	if(count < 0)
	{
		perror("Error:Read\n");
		return 1;
	}
	return 0;
}
