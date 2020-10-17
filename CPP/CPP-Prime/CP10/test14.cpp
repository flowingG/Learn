#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int func(const int);

int main()
{
	auto f = [](const int a, const int b){return a + b;};
	cout << "1+2" << " 19+23" << endl;
	cout << f(1, 2) << " " << f(19, 23) << endl;
	cout << func(2) << endl;
	return 0;
}

int func(const int a)
{
	int b;
	cout << "func1:" << endl;
	cin >> b;
//auto f = [a](const int b){return a + b;};
//	return f(b);
}
