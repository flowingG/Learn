#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
	int i = 0;
	string s1;
	while(getline(cin, s1))
	{
		if(!s1.empty())
		{
			int size = s1.size();
			while(size-- > 0)
			{
				cout << s1[i++];
			}
			cout << s1[i] << endl;
		}
		i = 0;
	}
	return 0;	
}
