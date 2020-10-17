#include <iostream>

int main(){
	using namespace std;
	int a = 9;
	int *b;
	b = &a;
	cout << *b << endl;
	return 0;
}
