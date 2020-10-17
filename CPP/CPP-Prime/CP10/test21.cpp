#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	int a = 20;
	auto f = [&]()mutable->bool{
		--a;
		if(a == 0) 
			return true; 
		else
			return false;};
	while(!f())
		cout << a << " ";
	cout << endl;
	return 0;
}
