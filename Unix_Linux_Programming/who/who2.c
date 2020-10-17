/* who2.c - read utmp and list info therein
 * 	  - suppresses empty records
 *	  - formats time nicely
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
	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}
	while(read(utmpfd, &current_record, reclen) == reclen)
		show_info(&current_record);
	close(utmpfd);
	return 0;
}

/*
 * show info()
 * display contents of the utmp struct in human readable from
 * * note * these sizes should not be hardwired
 */ 
void show_info(struct utmp *utbufp)
{
	if(utbufp->ut_type != USER_PROCESS)
		return;
	printf("%-8.8s", utbufp->ut_name);
	printf(" ");
	printf("%-8.8s", utbufp->ut_line);
	printf(" ");
	showtime(utbufp->ut_time);
	printf(" ");
	printf("%-8.8s", utbufp->ut_id);
	printf(" ");
	printf("%-5d", utbufp->ut_pid);
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
