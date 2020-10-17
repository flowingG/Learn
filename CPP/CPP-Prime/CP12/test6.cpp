#include <iostream>
#include <memory>
#include <vector>
#include <memory>

using std::shared_ptr;
using std::make_shared;
using std::ostream;
using std::istream;
using std::cin;
using std::cout;
using std::endl;
using std::vector;

shared_ptr<vector<int>> func1();
shared_ptr<vector<int>> func2(istream &is);
void print(ostream &os, shared_ptr<vector<int>> v);

int main()
{
	print(cout, func2(cin));
	return 0;
}

shared_ptr<vector<int>> func1()
{
	auto v = make_shared<vector<int>>();
	return v;
}

shared_ptr<vector<int>> func2(istream &is)
{
	auto v = func1();
	int tmp;
	while(is >> tmp)
		v->push_back(tmp);
	return v;
}

void print(ostream &os, shared_ptr<vector<int>> v)
{
	auto v1 = *v;
	for(auto s : v1)
		os << s << " ";
	os << endl;
}
