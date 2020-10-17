#include <iostream>
#include <vector>
#include <array>

int main(){
	using namespace std;             
	vector<int> a1(4);
	array<int, 4> a2, a3;
	for(int i = 0; i < 4; i++){
		cin >> a1[i];
		a2[i] = a1[i];
	}
	a3 = a2;
	cout << a1[0] << a1[1] << a1[2] << a1[3] << endl;
	cout << a2[0] << a2[1] << a2[2] << a2[3] << endl;
	cout << a3[0];
	return 0;
}
