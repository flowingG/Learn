#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main()
{
	vector<int> v;
	cout << v.size() << " " << v.capacity() << endl;
	for(vector<int>::size_type ix = 0; ix != 24; ++ix)
		v.push_back(ix);
	v.reserve(59);
	while(v.size() != v.capacity())
		v.push_back(1);
	v.push_back(1);
	v.shrink_to_fit();
	cout << v.size() << " " << v.capacity() << endl;
	return 0;
}
