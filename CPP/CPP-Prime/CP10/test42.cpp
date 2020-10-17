#include <iostream>
#include <list>

using std::list;
using std::cin;
using std::cout;
using std::endl;

int main()
{
	list<int> li;
	int a;
	while(cin >> a)
		li.push_front(a);
	for(auto i : li)
		cout << i << " ";
	cout << endl;
	li.sort();
	li.unique();
	for(auto i : li)
		cout << i << " ";
	cout << endl;
	return 0;
}
