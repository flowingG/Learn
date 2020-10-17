/** od1.c
 *  version 1 of od - uses read, write and open with tunable buffer size
 *
 *  usage : od
 *  	  - od filename
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd, count;
	char  buff[4096];
	if(argc != 2 && argc != 1)
	{
		perror("usage:od or od filename\n");
		return;
	}
	if(argc == 1)
		fd = 0;
	if(argc == 2)
		fd = open(argv[1], O_RDONLY);
	while((count = read(fd, buff, 4096)) > 0)
	{
		for(int i = 0; i < count; i++)
		{
			printf("%0d ", buff[i]);
		}
	}
	if(count < 0)
	{
		perror("Error : Read\n");
		return;
	}
	return 0;
}
