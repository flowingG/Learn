/** ac1.c
 *  version 1 of ac - uses read and open with tunable buffer size
 *
 *  usage : ac
 */
#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFSIZE (sizeof(struct utmp) * 24)

void save_records(int, char *);

struct my_ut{
	short ut_type;
	char ut_user[UT_NAMESIZE];
	char ut_line[UT_LINESIZE];
	int32_t loginsec;
	int32_t logoutsec;
};

struct my_ut record[1024];
int nums = 0;

int main(){
	int fd, count;
	fd = open(WTMP_FILE, O_RDONLY);
	char buff[BUFFSIZE];
	while((count = read(fd, buff, BUFFSIZE)) > 0)
		save_records(count, buff);
	int32_t sum = 0;
	for(count = 0; count < nums - 1; count++)
	{
		sum += record[count].logoutsec - record[count].loginsec;
	}
	printf("total   %.2f\n", (double)sum / 60.0 / 60.0);
}

void save_records(int count, char *buff)
{
	int i, j;
	struct utmp* ut;
	for(i = 0; i < count; i += sizeof(struct utmp))
	{
		ut = (struct utmp *)&buff[i];
		if(ut->ut_type == DEAD_PROCESS)
		{
			for(j = 0; j < nums; j++)
			{
				if(record[j].ut_type == USER_PROCESS && \
				   strcmp(record[j].ut_line, ut->ut_line) == 0 && \
				   ut->ut_tv.tv_sec > record[j].loginsec)
				{
					record[j].logoutsec = ut->ut_tv.tv_sec;
					record[j].ut_type = DEAD_PROCESS;
				}
			}
		}
		else if((ut->ut_type == RUN_LVL && strncmp(ut->ut_user, "shutdown", 8) == 0) \
			 || (ut->ut_type == BOOT_TIME && strncmp(ut->ut_user, "reboot", 6) == 0))
		{
			for(j = 0; j < nums; j++)
			{
				if(record[j].ut_type == USER_PROCESS && ut->ut_tv.tv_sec > \
						record[j].loginsec)
				{
					record[j].logoutsec = ut->ut_tv.tv_sec;
					record[j].ut_type = ut->ut_type;
				}
			}
		}
		else if(ut->ut_type == USER_PROCESS)
		{
			for(j = 0; j < nums; j++)
			{
				if(strcmp(record[j].ut_user, ut->ut_user) == 0 && \
				   record[j].loginsec == ut->ut_tv.tv_sec)
					break;
			}
			if(j == nums)
			{
				record[nums].ut_type = ut->ut_type;
				strcpy(record[nums].ut_user, ut->ut_user);
				strcpy(record[nums].ut_line, ut->ut_line);
				record[nums].loginsec = ut->ut_tv.tv_sec;
				nums++;
			}
		}
	}
}
