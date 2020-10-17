#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::ostream;

template<typename T> ostream &print(ostream &os, const T &t)
{
	return os << t;
}

template<typename T, typename ... Args> ostream &print(ostream &os, const T &t, const Args& ... rest)
{
	os << t << " ";
	print(os, rest...);
}

int main()
{
	int i = 42;
	string s = "Hwwwww";
	print(cout, i, s, 42, 13, 15);
	return 0;
}
