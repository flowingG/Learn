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
	//for(i = 0; i < v.size() - 1; i++)
	//	cout << v[i] + v[i + 1] << endl;
	
	for(i = 0; i < v.size() - 1; i++)
		cout << v[i] + v[v.size() - 1 - i] << endl;

	return 0;
}
