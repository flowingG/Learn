#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::istringstream;
using std::ostringstream;

struct PersonInfo{
	string name;
	vector<string> phones;
};

int name()
{
	string line, word;
	vector<PersonInfo> people;
	while(getline(cin, line))
	{
		PersonInfo info;
		istringstream record(line);
		record >> info.name;
		while(record >> word)
			info.phones.push_back(word);
		people.push_back(info);
	}
}
