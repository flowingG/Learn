#include <iostream>
#include <iterator>
#include <vector>
#include <numeric>
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::istream_iterator;

void printVecInt(vector<int> v);

int main()
{
	istream_iterator<int> input(cin), eof;
	
	vector<int> v;
	std::copy(input, eof, back_inserter(v));
	printVecInt(v);
	sort(v.begin(), v.end());
	printVecInt(v);
	return 0;
}

void printVecInt(vector<int> v){
	auto cbegin = v.cbegin();
	while(cbegin != v.cend())
		cout << *cbegin++ << " ";
	cout << endl;
}
