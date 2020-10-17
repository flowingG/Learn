#include <fstream>
#include <iostream>
#include <string>
#include <cstring>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;
using std::string;
using std::cerr;

int main(int argc, char **argv)
{
	if(argc != 3)
		cerr << "Parameter Error" << endl;
	if(strcmp(argv[1], argv[2]) == 0)
		cerr << "Input File is the same as Output File" << endl;
	ifstream input(argv[1]);
	ofstream output(argv[2], ofstream::app);
	string buff;
	while(true)
	{
		if(input.eof())
			break;
		string tmp;
		getline(input, tmp);
		buff += tmp + '\n';
	}
	buff[buff.size() - 1] = '\0';
	output << buff;
	return 0;
}
