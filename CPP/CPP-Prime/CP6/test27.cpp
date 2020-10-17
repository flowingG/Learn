#include <iostream>
#include <initializer_list>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::initializer_list;

int sum_list(initializer_list<int> list);

int main()
{
//	vector<int> v;
//	int a;
//	while(cin >> a)
//		v.push_back(a);

	int a[5], i;
	i = 0;
	while(cin >> a[i])
		++i;
	int sum = sum_list({a[0], a[1], a[2], a[3], a[4]});
	cout << sum << endl;
	return 0;
}

int sum_list(initializer_list<int> list)
{
	int sum = 0;
	for(auto i = list.begin(); i != list.end(); ++i)
		sum += *i;
	return sum;
}
