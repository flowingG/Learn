#include <iostream>
#include <iomanip>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;

int main()
{
	cout << cout.precision() <<  sqrt(2.0) << endl;
	cout.precision(12);
	cout << cout.precision() << sqrt(2.0) << endl;
	return 0;	
}
