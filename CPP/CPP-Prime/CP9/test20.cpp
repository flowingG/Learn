#include <iostream>
#include <list>
#include <deque>

using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::deque;

int main()
{
	list<int> li;
	deque<int> de1;
	deque<int> de2;
	for(int i = 1; i <= 100; ++i)
		li.push_front(i);
	for(auto i : li)
	{
		if(i % 2 == 0)
			de1.push_back(i);
		else
			de2.push_back(i);
	}
	auto be = de1.begin();
	auto en = de1.end();
	cout << "de1:" << endl;
	for(; be != en; ++be)
		cout << *be << " ";
	cout << endl << "de2:" << endl;
	for(auto i : de2)
		cout << i << " ";
	cout << endl;
	return 0;
}
