#include <iostream>
#include <vector>
#include <list>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::list;

template<typename T, typename N> int my_find(T &t, N &a)
{
	int index = 0;
	for(; index < t.size(); index++)
		if(a == t[index])
			return index;
	return -1;
}

int main()
{
//	vector<int> v;
	list<int> li;
	int a;
	cout << "Input: " << endl;
	while(cin >> a)
		li.push_back(a);
	cout << "Find:" << endl;
	cin >> a;
	if((a = my_find(li, a)) >= 0)
		cout << li[a] << "  " << a << endl;
	else
		cout << "Can't" << endl;
	return 0;
}
