#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::string;

int main()
{
	//vector<int> v;
	//int i;
	//while(cin >> i)
	//{
	//	v.push_back(i);
	//	cout << i << endl;
	//}

	vector<string> str;
	string s;
	while(cin >> s)
	{
		str.push_back(s);
	}
	for(auto i : str)
		cout << i << endl;
	return 0;
}
