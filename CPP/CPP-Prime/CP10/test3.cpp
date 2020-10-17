#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

using std::cin;
using std::cout;
using std::vector;
using std::cerr;
using std::endl;

int main()
{
	vector<int> v;
	for(int i = 0; i < 10; i++)
		v.push_back(i);
	auto sum = accumulate(v.cbegin(), v.cend(), 0);
	cout << sum << endl;
}
