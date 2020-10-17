#include <iostream>
#include <functional>
#include <map>
#include <string>

using std::function;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::map;

int main()
{
	map<string, function<int(int, int)>> binops;
	function<int(int, int)> f1 = [](int i, int j) { return i + j; };
	binops.insert({"+", f1});
	cout << binops["+"](10, 2);
	return 0;
}
