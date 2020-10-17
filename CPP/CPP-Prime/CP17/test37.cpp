#include <iostream>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;
using std::ifstream;
using std::ofstream;

int main(int argc, char *argv[])
{
	char buf[10];
	ifstream input(argv[1]);
	while(true)
	{
		if(input.getline(buf, 10, '\n'))
		{
			cout << buf << endl;
		}
		else{
			cout << buf << endl;
			break;
		}
	}
	return 0;		
}
