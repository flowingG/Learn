#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

using std::ifstream;
using std::ofstream;
using std::istream_iterator;
using std::cerr;
using std::endl;
using std::vector;

int main(int argc, char **argv)
{
	if(argc != 4)
		cerr << "Argumant Error!" << endl;
	ifstream input(argv[1]);
	vector<int> v;
	istream_iterator<int> in(input), eof;
	std::copy(in, eof, back_inserter(v));
	ofstream out(argv[2]);
	ofstream out2(argv[3]);
	auto be = v.cbegin();
	while(be != v.cend())
	{
		if(*be % 2)
			out << *be++ << " ";
		else
			out2 << *be++ << '\n';
	}
	return 0;
}
