#include <iostream>

inline double square(double x){return x * x;}

int main(){
	using namespace std;
	double a;
	double b;
	a = square(5.5);
	b = square(4.4);
	cout << a << endl << b << endl;
	return 0;
}
