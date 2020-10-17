#include <iostream>
#include <vector>
#include <string>
#include <map>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::map;

int main()
{
	map<string, vector<string>> homes;
	string name1;
	string name2;
	vector<string> homenames;
	while(true)
	{
		cout << " Enter your name:" << endl;
		cin >> name1;
		cout << " Enter your homenames:" << endl;
		cin >> name2;
		homes[name1].push_back(name2);
		char c;
		cout << "Print y/n?" << endl;
		cin >> c;
		if(c == 'n')
			break;
	}
	for(auto &s : homes)
	{
		cout << s.first << " : " << endl;
		for(auto &s2 : s.second)
			cout << s2 << endl;
	}
	return 0;
}
