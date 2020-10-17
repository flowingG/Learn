/* sleep1.c
 * purpose	show how sleep works
 * usage	sleep1
 * outline 	set handler, sets alarm, pauses, then returns
 */
#include <stdio.h>
#include <signal.h>

void wakeup(int);

int main()
{
	printf("about to sleep for 4 seconds\n");
	signal(SIGALRM, wakeup);
	alarm(4);
	pause();
	return 0;
}

void wakeup(int sig)
{
#ifndef SHHHH
	printf("Alarm received from kernel\n");
#endif
}
