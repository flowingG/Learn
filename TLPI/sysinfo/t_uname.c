#define	_GNU_SOURCE
#include <sys/utsname.h>
#include "../lib/tlpi_hdr.h"

int main(int argc, char *argv[]){
	struct utsname t;
	if(uname(&t) == -1)
		errExit("uname");
	printf("Node name: %s\n", t.nodename);
	printf("System name: %s\n", t.sysname);
	printf("Release: %s\n", t.release);
	printf("Version: %s\n", t.version);
	printf("Machine: %s\n", t.machine);
#ifdef _GNU_SOURCE
	printf("Domain name:%s\n", t.domainname);
#endif
	exit(EXIT_SUCCESS);
}
