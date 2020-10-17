#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

bool find(const vector<int>, int);

int main()
{
	vector<int> v;
	for(int i = 0; i < 100; i++)
		v.push_back(i);
	int a;
	while(cin >> a)
		cout << ((find(v, a) == true) ? "TRUE" : "FALSE") << endl;
	return 0;	
}

bool find(const vector<int> v, int a)
{
	auto be = v.begin();
	auto en = v.end();
	for(; be != en; ++be)
	{
		if(*be == a)
			return true;
	}
	return false;
}
