#include <iostream>
#include <fstream>
#include <cstdlib>
const int SIZE = 60;

int main(){
	using namespace std;
	int value, count, sum;
	char filename[SIZE];
	ifstream infile;
	cin.getline(filename, SIZE);
	infile.open(filename);
	if(infile.is_open()){
		infile >> value;
		while(infile.good()){
			count++;
			sum += value;
			infile >> value;
		}
		if(infile.eof())
			cout << "A" << endl;
		else if(infile.fail())
			cout << "E" << endl;
		else 
			cout << "?" << endl;
	}
	cout << count << " " << sum << " " << endl;
	infile.close();
	return 0;
}
