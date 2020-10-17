#include <iostream>
#include <numeric>
#include <iterator>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::istream_iterator;
using std::ostream_iterator;

int main()
{
	istream_iterator<int> in(cin), eof;
	cout << accumulate(in, eof, 0) << endl;
	ostream_iterator<int> out_iter(cout, " ");
	for(int i = 0; i < 200; ++i)
		out_iter = i;
	cout << endl;
	return 0;
}
