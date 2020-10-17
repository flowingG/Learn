#include <iostream>
#include <string>
#include <sstream>

using std::istream;
using std::ostream;
using std::istringstream;
using std::ostringstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;

istream &readToPrint(istream &is);

int main()
{
	istringstream s("asjofasnfjsdbd\nuifhs\n13924y12\n");
	readToPrint(s);
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
