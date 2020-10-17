#include <iostream>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::runtime_error;

int main()
{
	for(int i, j; cout << "Input tow Inter:", cin >> i >> j;)
	{
		try{
			if(j == 0)
				throw runtime_error("Divistor is 0");
		}catch(runtime_error err)
		{
			cout << err.what() << "\nRRRRRR" << endl;
			char c;
			cin >> c;
			if(c == 'n')
				break;
			else if(c == 'y')
				continue;
		}
		cout << i / j << endl;
	} 
	return 0;
}
