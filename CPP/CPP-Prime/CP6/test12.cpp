#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void swap(int &a, int &b);

int main()
{
	int a = 1, b = 2;
	cout << "a:" << a << "b:" << b << endl;
	swap(a, b);
	cout << "a:" << a << "b:" << b << endl;
	return 0;
}

void swap(int &a, int &b)
{
	int tmp;
	tmp = a;
	a = b;
	b = tmp;
}
