/* who2_2.c - who am i | whoami
 * 	    - read utmp and list info therein
 * 	    - suppresses empty records
 *	    - formats time nicely
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHOWHOST

void showtime(long);
void show_info(struct utmp *);

int main(int argc, char **argv)
{
	struct utmp current_record;
	int utmpfd;
	int reclen = sizeof(current_record);
	if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1)
	{
		perror(UTMP_FILE);
		exit(1);
	}
	if(argc == 2 || argc == 4)
	{
		if(argc == 2 && (strcmp(argv[1], "whoami") == 0))
		{
			while(read(utmpfd, &current_record, reclen) == reclen)
			{
				if(strcmp(current_record.ut_line, "tty2") == 0)
				{
					printf("%s\n", current_record.ut_name);
					break;
				}
			}
		}
		else if(argc == 4 && (strcmp(argv[1], "who") == 0) && (strcmp(argv[2], "am") == 0) && (strcmp(argv[3], "i") == 0))
		{
			while(read(utmpfd, &current_record, reclen) == reclen)
				if(strcmp(current_record.ut_line, "tty2") == 0)
				{
					show_info(&current_record);
					break;
				}
		}
		else
		{
			perror("usage:who am i or whoami\n");
			exit(1);
		}
		return 0;
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
