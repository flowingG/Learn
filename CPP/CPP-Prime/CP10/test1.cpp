#include <iostream>
#include <algorithm>
#include <vector>

using std::vector;

int main()
{
	vector<int> v;
	for(int i = 0; i < 100; ++i)
		for(int j = i; j < 100; j++)
			v.push_back(j);
	while(true)
	{
		int i;
		std::cin >> i;
		auto count = std::count(v.cbegin(), v.cend(), i);
		std::cout << count << std::endl;
	}
	return 0;
}
