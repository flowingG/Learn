#include <iostream>
#include <cctype>

int main(){
	using namespace std;
	char ch;
	cin.get(ch);
	if(isalpha(ch))
		cout << ch;
	else if(isdigit(ch))
		cout << "as" << ch;
	else if(ispunct(ch))	
		cout << "11 " << ch;
	return 0;
}
