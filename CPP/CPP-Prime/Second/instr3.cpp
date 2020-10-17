#include <iostream>

int main(){
	using namespace std;
	const int ArSize = 20;
	char name[ArSize];
	char dessert[ArSize];

	cout << "enter your name\n";
	cin.get(name, ArSize).get();
	cout << "enter your dessert\n";
	cin.get(dessert, ArSize).get();
	cout << name << " " << dessert;
	return 0;
}
