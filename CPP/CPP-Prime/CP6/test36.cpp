#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

typedef string str[10];
string s[10];

string (&func())[10];
str &func1();
auto func2()->string(&)[10];
decltype(s) &func3();

int main()
{
	
}

