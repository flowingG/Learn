#include <iostream>

int main(){
	using namespace std;
	int *pt = new int;
	*pt = 1000;
	cout << *pt << endl;
	delete pt;
	return 0;
}
