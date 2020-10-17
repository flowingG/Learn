#include <iostream>
#include <tuple>

using std::cin;
using std::cout;
using std::endl;
using std::tuple;
using std::get;

int main()
{
	tuple<int, int, int> a{10, 20, 30};
	cout << get<0>(a) << get<1>(a) << get<2>(a) << endl;
	return 0;
}
