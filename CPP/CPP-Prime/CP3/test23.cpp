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
	vector<int> v;
	int t;
	for(int i = 0; i < 10; i++)
	{
		cin >> t;
		v.push_back(t);
	}
	for(auto i = v.begin(); i != v.end(); ++i)
	{
		*i *= 2;
	}
	for(auto i = v.begin(); i != v.end(); ++i)
		cout << *i << " ";
	cout << endl;
	return 0;
}
