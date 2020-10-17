#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
	string s1;
	getline(cin, s1);
	for(char &c : s1)
		if(isalpha(c))
			c = 'X';
	cout << s1 << endl;
	return 0;
}
