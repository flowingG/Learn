#include <iostream>
#include <vector>

using namespace std;

int main()
{
	vector<int> a;
	vector<int> b;
	for(int i = 0; i < 99; i++)
	{
		a.push_back(i);
		b.push_back(i + 2);
	}
	cout << (a == b) << endl;
	return 0;
}
