/* forkdemo1.c
 * shows how fork creates two processes, distinguishable
 * by the different return values from fork()
 */
#include <stdio.h>

int main(){
	int ret_from_fork, mypid;
	mypid = getpid();
	printf("Before: mypid is %d\n", mypid);
	ret_from_fork = fork();
	wait();
	printf("After: mypid is %d, fork() said %d\n", getpid(), ret_from_fork);
	return 0;
}
