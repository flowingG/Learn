#include <iostream>
#include <climits>

int main(){
	using namespace std;
	int n_int = INT_MAX;
	short n_short = SHRT_MAX;
	long n_long = LONG_MAX;
	long long n_llong = LLONG_MAX;
	
	cout << n_int << "    " << sizeof(n_int) << endl;
	cout << n_short << "   " << sizeof n_short << endl;
	cout << n_long << "   " << sizeof n_long << endl;
	cout << n_llong << "   " << sizeof n_llong << endl;
	cout.put('s');
	
	return 0;
}
