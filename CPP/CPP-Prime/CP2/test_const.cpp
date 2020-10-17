#include <iostream>
int main(){
	typedef char *d;
	const d str = 0;
	*str = 0;
	int a = 5;
	int &b = a;
	const int &c = a;
	std::cout << a << " " << b << " " << c << std::endl;
	b = 4;
	std::cout << a << " " << b << " " << c << std::endl;
	return 0;
}
