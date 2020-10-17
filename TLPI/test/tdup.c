#include <unistd.h>

int main(int argv, char *argc[]){
	int fd;
	char a[] = "adasdada";
	fd = dup(1);

	write(fd, a, 8);

	return 0;
}
