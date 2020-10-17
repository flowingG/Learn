#include <iostream>
#include <list>
#include <algorithm>
#include <numeric>

using std::cin;
using std::cout;
using std::endl;
using std::list;

int main()
{
	list<int> li;
	int a;
	while(cin >> a)
		li.push_front(a);
	auto fi = find(li.crbegin(), li.crend(), 0);
	cout << *fi << " " << *(++fi) << endl;
	return 0;
}
