#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>

using std::istringstream;
using std::ostringstream;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

int main()
{
	string buff;
	ifstream input;
	cout << "Please Input Your Filename:" << endl;
	while(cin >> buff)
	{
		try
		{
			input.open(buff);
			if(!input.good())
				throw runtime_error("The File Not Exist.");
			break;
		}catch(runtime_error err)
		{
			cout << err.what() << "\nTry Again? Enter y or n" << endl;
			char c;
			cin >> c;
			if(!cin || c == 'n')
				break;
			cout << "Please Input Your Filename:" << endl;	
		}
	}
	vector<string> vs;
	while(getline(input, buff))
		vs.push_back(buff);	
	cout << ">" << endl;
	for(auto &s : vs)
	{
		istringstream rs(s);
		while(rs >> buff)
			cout << buff << endl;
	}
	return 0;
}
