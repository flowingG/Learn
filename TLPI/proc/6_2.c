#include <stdio.h>
#include <setjmp.h>

static jmp_buf env;

void f1(){
	int a, b, c;
	a = 1, b = 2, c = 3;
	if(setjmp(env) != 0)
		printf("longjmp() %d%d%d\n", a, b, c);
	else
		printf("setjmp() %d%d%d\n", a, b, c);
}

void f2(){
	longjmp(env, 1);
}

int main(int argc, char *argv[]){
	f1();
	printf("f1() xxxx\n");
	f2();
	printf("end\n");
}
