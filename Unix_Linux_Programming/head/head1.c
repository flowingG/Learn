/** head1.c
 *  version 1 of head - uses read, write, lseek and open with tunable buffer size
 *
 *  usage : - head
 *          - head filename
 *  	    - head -n count filename 
 *  	    - head filename -n count
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc!= 1 && argc != 2 && argc != 4)
	{
		perror("usage : head or head filename\n");
		return 1;
	}
	int inputfd, linesum;
	if(argc == 1)
	{
		inputfd = 0;
		linesum = 10;
	}
	if(argc == 2)
	{
		inputfd = open(argv[1], O_RDONLY);
		linesum = 10;
	}
	else if(argc == 4)
	{
		if(strcmp(argv[1], "-n") == 0)
		{
			linesum = atoi(argv[2]);
			inputfd = open(argv[3], O_RDONLY);
		}
		else if(strcmp(argv[2], "-n") == 0)
		{
			linesum = atoi(argv[3]);
			inputfd = open(argv[1], O_RDONLY);
		}
		else
		{
			perror("usage : head -n count filename or head filename -n count\n");
			return 1;
		}
	}
	int count, index, i, outputfd = 1;
	char buff[4096];
	while((count = read(inputfd, buff, 4096)) > 0)
	{
		for(i = 0; i < count; i++)
		{
			if(buff[i] == '\n')
			{
				index = i + 1;
				linesum--;
			}
			if(linesum == 0)
				break;
		}
		write(outputfd, buff, index);
		if(linesum != 0)
			lseek(inputfd, index, SEEK_SET);
		else
			break;
	}
	if(count < 0)
	{
		perror("Error:Read\n");
		return 1;
	}
	return 0;
}
