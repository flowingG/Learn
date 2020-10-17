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
	vector<string> text;
	string s;
	while(getline(cin, s))
		text.push_back(s);
	auto *s1 = &text[0];
	for(auto c = s1->begin(); c != s1->end(); ++c)
		if(islower(*c))
			*c = toupper(*c);
	for(auto it = text.cbegin(); it != text.cend() && !it->empty(); ++it)
	{
		cout << *it << endl;
	}
	return 0;
}
