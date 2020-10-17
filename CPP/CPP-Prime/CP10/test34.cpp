#include <iostream>
#include <iterator>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main()
{
	vector<int> v;
	int i;
	while(cin >> i)
		v.push_back(i);
//	for(auto end = v.cend(); end != v.cbegin();)
//		cout << *(--end) << " ";
	for(auto begin = v.crbegin(); begin != v.crend(); ++begin)
		cout << *begin << " ";
	cout << endl;
	return 0;
}
