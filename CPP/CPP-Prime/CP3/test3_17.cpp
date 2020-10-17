#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{
	vector<char> v;
	string s;
	while(getline(cin, s))
	{
		for(auto c : s)
		{
			if(isalpha(c))
				c = toupper(c);
			v.push_back(c);
		}
	}
	for(auto t : v)
		cout << t << endl;
	return 0;
}
