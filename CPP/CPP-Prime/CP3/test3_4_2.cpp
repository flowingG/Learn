#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

int main()
{
	string s1, s2;
	getline(cin, s1);
	getline(cin, s2);
	if(s1.size() != s2.size())
		s1.size() > s2.size() ? cout << s1 << endl : cout << s2 << endl;
	return 0;
}
