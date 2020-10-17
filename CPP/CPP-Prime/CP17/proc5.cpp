#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::hex;
using std::oct;
using std::dec;
using std::showbase;
using std::noshowbase;

int main()
{
	cout << showbase;
	cout << 1024 << endl;
	cout << oct << 1024 << endl;
	cout << hex << 1024 << endl;
	cout << dec << 1024 << endl;
	cout << noshowbase;
	return 0;
}
