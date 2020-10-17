#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
	string s1;
	getline(cin, s1);
	//while
	//decltype(s1.size()) size = 0;
	//while(size < s1.size())
	//{
	//	if(isalpha(s1[size]))
	//		s1[size++] = 'X';
	//	else
	//		size++;
	//}
	
	//for
	for(decltype(s1.size()) i = 0; i < s1.size(); i++)
		if(isalpha(s1[i]))
			s1[i] = 'X';
	
	cout << s1 << endl;
	return 0;
}
