#include <iostream>
int main(){
	int  i = 0;
	int &a = i;
	double b = 2.2;
	std::cout << a << std::endl;
	a = 200;
	b = a;
	std::cout << a << "  " << i << "  " << b << std::endl;
	return 0;
}
