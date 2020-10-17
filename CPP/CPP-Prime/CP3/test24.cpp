#include <iostream>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{
	vector<int> v;
	int i;
	while(cin >> i)
		v.push_back(i);
	//for(auto t = v.begin(); t + 1 != v.end(); t++)
	//	cout <<  *t + *(t + 1) << endl;
	i = 1;	
	for(auto t = v.cbegin(); t + 1 != v.cend(); ++t, ++i)
		cout << *t + *((v.cend() - t) + t - i) << endl;

	return 0;
}
