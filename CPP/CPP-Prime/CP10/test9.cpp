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
	int a;
	while(cin >> a)
		v.push_back(a);
	for(auto i : v)
		cout << i << " ";
	sort(v.begin(), v.end());
	cout << endl << "sorted:" << endl;
	for(auto i : v)
		cout << i << " ";
	auto end_unique = unique(v.begin(), v.end());
	cout << endl << "uniqued:" << endl;
	for(auto i : v)
		cout << i << " ";
	v.erase(end_unique, v.end());
	cout << endl << "erased:" << endl;
	for(auto i : v)
		cout << i << " ";
	cout  << endl;
	return 0;
}
