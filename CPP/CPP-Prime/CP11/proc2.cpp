#include <iostream>
#include <vector>
#include <set>

using std::multiset;
using std::set;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main()
{
	vector<int> ivec;
	for(vector<int>::size_type i = 0; i != 10; ++i)
	{
		ivec.push_back(i);
		ivec.push_back(i);
	}
	set<int> iset(ivec.cbegin(), ivec.cend());
	multiset<int> miset(ivec.cbegin(), ivec.cend());
	cout << ivec.size() << " " << iset.size() << miset.size() << endl;
	return 0;
}
