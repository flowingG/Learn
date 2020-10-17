/* waitdemo1.c - shows how parent gets child status */
#include <stdio.h>
#include <stdlib.h>

#define DELAY 5

void child_code(int);
void parent_code(int);

int main(){
	int newpid;
	printf("before: my pid is %d\n", getpid());
	if((newpid = fork()) == -1)
		perror("fork");
	else if(newpid == 0)
		child_code(DELAY);
	else
		parent_code(newpid);
	return 0;
}

/* new process takes a nap and then exits */
void child_code(int delay){
	printf("child %d here. will sleep for %d seconds.\n", getpid(), delay);
	sleep(delay);
	printf("child done.about to exit\n");
	exit(17);
}

/* parent waits for child then prints a message */
void parent_code(int childpid){
	int wait_tv, child_status;
	int high, low, bit;
	wait_tv = wait(&child_status);
	printf("done waiting for %d. Wait returned: %d\n", childpid, wait_tv);
	high = child_status >> 8;
	low = child_status & 0x7F;
	bit = child_status & 0x80;
	printf("status: exit = %d, sig = %d, core = %d\n", high, low, bit);
}
