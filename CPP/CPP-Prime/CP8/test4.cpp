#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using std::vector;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::cerr;

int main(int argc, char **argv)
{
	if(argc != 2)
		cerr << "Parameter Error!" << endl;
	ifstream input(argv[1]);
	string s;
	vector<string> vs;
	while(getline(input, s))
		vs.push_back(s);
	for(auto tmp : vs)
		cout << tmp << endl;
	return 0;
}
