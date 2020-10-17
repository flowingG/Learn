#include <iostream>
#include <string>
using std::cin;
using std::cout;
using std::endl;
using std::string;
int main()
{
	//string word;
	//while(cin >> word)
	//	cout << word << endl;
	string line;
	while(getline(cin, line))
		if(!line.empty())
			cout << line << line.size() << endl;	

	return 0;
}
