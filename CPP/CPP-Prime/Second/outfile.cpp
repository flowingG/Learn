#include <iostream>
#include <fstream>

int main(){
	using namespace std;
	ofstream outFile;
	outFile.open("test.txt");
	char str[50];
	int year;
	cin.getline(str, 50);
	cin >> year;
	outFile << fixed;
	outFile.precision(2);
	outFile.setf(ios_base::showpoint);
	outFile << "My " << str << endl;
	outFile << year << endl;
	outFile.close();
	return 0;
}
