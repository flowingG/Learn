#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;

bool is_upper(const string &s);
void lower(string &s);

int main()
{
	string s;
	getline(cin, s);
	cout << s << endl;
	if(is_upper(s))
		lower(s);
	cout << s << endl;
	return 0;
}

bool is_upper(const string &s)
{
	for(auto c : s)
		if(isupper(c))
			return true;
}

void lower(string &s)
{
	for(auto &c : s)
		if(islower(c))
			c = toupper(c);
}
