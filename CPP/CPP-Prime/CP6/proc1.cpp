#include <iostream>

using std::cin;
using std::cout;
using std::endl;

void reset(int &i);

int main()
{
	int  i = 12;
	cout << i << endl;
	reset(i);
	cout << i << endl;
	return 0;
}

void reset(int &i)
{
	i = 0;
}
