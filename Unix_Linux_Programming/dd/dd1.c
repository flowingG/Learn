/** dd1.c
 *  version 1 of dd - uses read, write and open with tunable buffer size
 *
 *  usage : dd if=SRCPATH of=DESTPATH
 *  output : x+x records in
 *  	     x+x records out
 *	     xxx bytes (xxx B) copied, xxxxxxxs, xxxkb/s
 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv)
{
	int inputfd, outputfd, count;
	char buff[4096];
	if(argc != 3)
	{
		perror("usage: dd if=SRCPATH of=DESTPATH\n");
		return -1;
	}
	if(strncmp(argv[1], "if=", 3) != 0)
	{
		perror("Error! dd 'if='\n");
		return -1;
	}
	if(strncmp(argv[2], "of=", 3) != 0)
	{
		perror("Error! dd if=xxx 'of='\n");
		return -1;
	}
	inputfd = open(&argv[1][0] + 3, O_RDONLY);
	outputfd = open(&argv[2][0] + 3, O_WRONLY);
	int inbytes, outbytes;
	inbytes = 0, outbytes = 0;
	time_t nowtime = time(NULL);
	while((count = read(inputfd, buff, 4096)) > 0)
	{
		inbytes += count;
		outbytes += write(outputfd, buff, count);
	}
	nowtime = time(NULL) - nowtime;
	printf("%d+%d records in\n", inbytes / 512, (inbytes % 512 == 0 ? 0 : 1));
	printf("%d+%d records out\n", outbytes / 512, (outbytes % 512 == 0 ? 0 : 1));
	printf("%dbytes (%d B) copied, %fs, %fKB/s\n", inbytes, inbytes / 1024,\
	(double)nowtime , (double)inbytes / 1024.0 / (double)nowtime);
}
