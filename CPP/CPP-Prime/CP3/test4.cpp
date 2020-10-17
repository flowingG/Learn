#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

int main()
{
	string s1("Hello World!!!");
	//decltype(s1.size()) punct_cnt = 0;
	//for(auto c : s1)
	//	if(ispunct(c))
	//		++punct_cnt;
	//cout << punct_cnt << " " << s1 << endl;
	
	//2
	for(auto &c : s1)
		c = toupper(c);
	cout << s1 << endl;

	return 0;
}
