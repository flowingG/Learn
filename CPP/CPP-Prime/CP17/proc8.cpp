#include <iostream>

using std::cin;
using std::cout;
using std::endl;
using std::noskipws;
using std::skipws;

int main()
{
	char c;
	cin >> noskipws;
	cin >> c;
	cout << c << "a" << c << "b" << c << "d" << endl;
	return 0;
}
