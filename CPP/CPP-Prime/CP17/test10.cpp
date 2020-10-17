#include <iostream>
#include <bitset>

using std::cin;
using std::cout;
using std::endl;
using std::bitset;

int main()
{
	int i = 1, j = 1;
	bitset<32> bit;
	for(; i <= 21; i += j, j = i - j)
		bit.set(i);
	cout << bit << endl;
	return 0;
}
