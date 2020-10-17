#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

template<typename T> void print(T &t)
{
	for(auto a : t)
		cout << a << " ";
	cout << endl;
}

int main()
{
	int a[10];
	string b[10];
	for(int i = 0; i < 10; i++)
	{
		cin >> a[i] >> b[i];
	}
	print(a);
	print(b);
	return 0;
}
