#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int fact()
{
	static int count = 0;
	return count++;
}

int main()
{
	for(int i = 0; i != 10; i++)
		cout << fact() << endl;
}
