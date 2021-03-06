/* last2.c - read wtmp and list info therein
 * 	   - suppresses empty records
 *	   - formats time nicely
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST

void showtime(long);
void show_info(struct utmp *);

int main()
{
	struct utmp current_record;
	int utmpfd;
	int reclen = sizeof(current_record);
	if((utmpfd = open(WTMP_FILE, O_RDONLY)) == -1)
	{
		perror(WTMP_FILE);
		exit(1);
	}
	while(read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
}

/*
 * show info()
 * display contents of the wtmp struct in human readable from
 * * note * these sizes should not be hardwired
 */ 
void show_info(struct utmp *utbufp)
{
	if(utbufp->ut_type != USER_PROCESS)
	{	
		if(utbufp->ut_type != BOOT_TIME)
			return; 
	}
	if(utbufp->ut_type == BOOT_TIME)
	{
		if(strncmp(utbufp->ut_user, "reboot", 6) != 0)
			return;
	}
	printf("%-8.8s", utbufp->ut_name);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
	showtime(utbufp->ut_time);
	printf(" ");
	printf("%-8.8s", utbufp->ut_id);
	printf(" ");
  #ifdef SHOWHOST
 	printf("(%s)", utbufp->ut_host);
  #endif
	printf("\n");
}

/*
 * displays time in a format fit for human consumption
 * users ctime to build a string then picks parts out of it
 * Note: %12.12s prints a string 12 chars wides and LIMITS
 * it to 12 chars
 */
void showtime(long timeval)
{
	char *cp;
	cp = ctime(&timeval);
	printf("%12.12s", cp + 4);
}
