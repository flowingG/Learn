#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
	const string s = "Ssss,,,sss";
	for(auto &c : s)
		if(ispunct(c))
			c = 0;
	cout << s << endl;
	return 0;
}
