#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main()
{
	vector<int> v;
	for(int i = 0; i < 100; ++i)
		v.push_back(i);
	for(auto i : v)
		cout << i << " ";
	cout  << endl;
	fill_n(v.begin(), v.size(), 0);
	for(auto i : v)
		cout << i << " ";
}
