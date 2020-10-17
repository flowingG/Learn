#include <iostream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

bool find(const vector<int>, int);
int findNum(const vector<int>, int);

int main()
{
	vector<int> v;
	for(int i = 0; i < 100; i++)
		v.push_back(i);
	int a;
	while(cin >> a)
		a = findNum(v, a);
	cout << a << endl;
	return 0;	
}

bool find(const vector<int> v, int a)
{
	auto be = v.begin();
	auto en = v.end();
	for(; be != en; ++be)
	{
		if(*be == a)
			return true;
	}
	return false;
}

int findNum(const vector<int> v, int i)
{
	while(true)
	{
		auto be = v.begin();
		auto en = v.end();
		for(; be != en; ++be)
			if(*be == i)
				return i;	
		std::cerr << "Find Error!\n" << "Try Again?" << endl;
		char c;
		cin >> c;
		if(c != 'y' || c != 'Y')
			break;
	}
	return -1;
}
