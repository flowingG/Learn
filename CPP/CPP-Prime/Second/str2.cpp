#include <iostream>
#include <string>

int main(){
	using namespace std;
	string str1;
	string str2;
	str1 = "aaa";
	cout << str1 << endl;
	str2 = str1 + "sss";
	cout << str2 << endl;
	string str3 = str1 + str2;
	cout << str3 << endl;
	cout << str1.size()  << " " << str2.size() << " " << str3.size() << endl;

	return 0;
}
