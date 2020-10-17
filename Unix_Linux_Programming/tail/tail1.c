/*
 *  version 1 of tail - uses read, write, lseek and open with tunable buffer size
 *
 *  usage : - tail filename
 *  	    - tail -n count filename 
 *  	    - tail filename -n count
 */
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
	if(argc!= 1 && argc != 2 && argc != 4)
	{
		perror("usage : tail or tail filename\n");
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
			perror("usage : tail -n count filename or tail filename -n count\n");
			return 1;
		}
	}
	int count, index, i, outputfd = 1;
	char buff[4096];
	int alllines = 1;
	while((count = read(inputfd, buff, 4096)) > 0)
		for(i = 0; i < count; i++)
			if(buff[i] == '\n')
				alllines++;
	if(count < 0)
	{
		perror("Error : Read\n");
		return 1;
	}
	lseek(inputfd, 0, SEEK_SET);
	if(alllines > linesum)
	{
		int skiplines = alllines - linesum;
		while((count = read(inputfd, buff, 4096)) > 0)
		{
			if(skiplines == 0)
			{
				write(outputfd, buff, count);
				continue;
			}
			for(i = 0; i < count; i++)
			{
				if(buff[i] == '\n')
					skiplines--;
				if(skiplines == 0)
				{
					lseek(inputfd, -(count - i) - 1, SEEK_CUR);
					break;
				}
			}
		}
	}
	else
	{
		while((count = read(inputfd, buff, 4096)) > 0)
			write(outputfd, buff, count);
	}
	return 0;
}
