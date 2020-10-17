#include <iostream>
#include <string>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::runtime_error;

int main(int argc, char **argv)
{
	try
	{	if(argc != 3)
		{
			throw runtime_error("ERROR");
		}
	}catch(runtime_error err)
	{
		cout << err.what() << "ERRORRR" << endl;
	}
	string s = argv[1];
	s += argv[2];
	cout << s << endl;
	return 0;
}
