#include <iostream>

int main()
{
	using namespace std;
	auto f = []{return 42;};
	cout << f() << endl;
	return 0;
}
