#include <iostream>
#include <string>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;

int main()
{
	int a, b, c;
	try{
		cin >> a >> b;
	//if(b == 0)
	//	throw runtime_error("Invalid data");
	}catch(runtime_error err)
	{
		cout << err.what() << "wwww" << endl;
	}
	c = a / b;
	cout << c << endl;
	return 0;
}
