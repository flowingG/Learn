#include <iostream>
#include <string>

using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::endl;
using std::string;

istream &readToPrint(istream &is);

int main()
{
	readToPrint(cin);
	return 0;
}

istream &readToPrint(istream &is)
{
	string s;
	auto old_state = is.rdstate();	
	is.clear();
	while(true)
	{
		if(is.eof())
			break;
		string tmp;
		getline(is, tmp);
		 s = s + tmp + '\n';
	}
	s[s.size() - 1] = '\0';
	cout << s;
	is.setstate(old_state);
	return is;
}
