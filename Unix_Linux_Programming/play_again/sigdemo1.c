/* sigdemo1.c - shows how a signal handler works.
 * - run this and press Ctrl - C a few times
 */
#include <stdio.h>
#include <signal.h>

void f(int signum);

int main(){
	int i;
	signal(SIGINT, f);
	for(i = 0; i < 5; ++i){
		printf("Hello\n");
		sleep(10);
	}
	return 0;
}

void f(int signum){
	printf("OUCH!\n");
}
