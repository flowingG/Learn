#include <iostream>
#include <tuple>
#include <string>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::tuple;
using std::get;
using std::vector;
using std::string;
using std::pair;

int main()
{
	tuple<string, vector<string>, pair<string, int>> a{"aaa", {"bbb", "ccc"}, {"aaa", 5}};
	return 0;
}
