#include <iostream>
#include <iomanip>

using std::cin;
using std::cout;
using std::endl;
using std::left;
using std::right;
using std::internal;
using std::setw;

int main()
{
	int i = -16;
	double d = 3.14159;
	cout << "i: " << setw(12) << i << "next col" << '\n' << "d: " << setw(12) << d << "next col" << '\n';
	cout << left << "i: "   << setw(12) << i << "next col" << '\n' << "d: " << setw(12) << d << "next col" << '\n' << right;
	cout << right << "i: " << setw(12) << i << "next col" << '\n' << "d: " << setw(12) << d << "next col" << '\n';
	return 0;
}
