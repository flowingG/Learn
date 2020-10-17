#include <iostream>
#include <map>
#include <string>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::ifstream;

int main(int argc, char **argv)
{
	map<string, size_t> word_count;
	string word;
	ifstream input(argv[1]);
	while(input >> word)
		++word_count[word];
	for(const auto &w : word_count)
		cout << w.first << " occurs " << w.second << ((w.second > 1) ? " times " : " time") << endl;
	return 0;
}
