#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
	string s1, s2, s3, s4;
	getline(cin, s1);
	getline(cin, s2);
	getline(cin, s3);
	getline(cin, s4);
	s1 = s1 + " " + s2 + " " + s3 + " " + s4;
	cout << s1 << endl;
	return 0;
}
