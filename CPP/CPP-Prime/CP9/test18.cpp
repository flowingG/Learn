#include <iostream>
#include <string>
#include <deque>
#include <list>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::deque;
using std::list;

int main()
{
	string ins;
/*	deque<string> de;
	while(getline(cin, ins))
		de.push_back(ins);
*/
	list<string> li;
	while(getline(cin, ins))
		li.insert(li.begin(), ins);
	for(auto &l : li)
		cout << l << endl;
	return 0;
}
