#include <iostream>
#include <string>
#include <vector>
#include <utility>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::pair;

int main()
{
	vector<pair<string, int>> vec;
	string s;
	int a;
	cout << "Input a string:";
	while(cin >> s)
	{
		cout << "Input a int:";
		cin >> a;
		pair<string, int> pa{s, a};
		vec.push_back(pa);
		cout << " Input a string:";
	}
	cout << endl;
	for(auto p : vec)
		cout << p.first << " " << p.second << endl;
	return 0;
}
