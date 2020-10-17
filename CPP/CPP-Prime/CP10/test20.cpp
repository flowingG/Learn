#include <iostream>
#include <numeric>
#include <algorithm>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

int main()
{
	vector<int> v;
	for(int i = 0; i < 100; i++)
		v.push_back(i);
	auto count = count_if(v.cbegin(), v.cend(), [](const int a){return a % 2 == 0;});
	cout << count << endl;
	return 0;
}
