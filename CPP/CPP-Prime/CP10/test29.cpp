#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iterator>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::istream_iterator;

int main(int argc, char **argv)
{
	ifstream input(argv[1]);
	istream_iterator<string> in(input), eof;
	vector<string> v;
	std::copy(in, eof, back_inserter(v));
	for(auto s : v)
		cout << s << endl;
	return 0;
}
