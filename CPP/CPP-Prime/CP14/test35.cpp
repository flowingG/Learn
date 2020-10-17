#include <iostream>
#include <string>
#include <vector>

using std::vector;
using std::string;
using std::istream;
using std::cin;
using std::cout;
using std::endl;

class InputString{
public:
	InputString(istream &in = std::cin, const string str = ""):is(in), s(str){ }
	string &operator()(string &str)
	{
		getline(is, str);
		return str;
	}
private:
	istream &is;
	string s;
};

int main()
{
	InputString in;
	string s;
	vector<string> vs;
	while(in(s) != "")
		vs.push_back(s);
	for(auto v : vs)
		cout << v << endl;
	return 0;
}
