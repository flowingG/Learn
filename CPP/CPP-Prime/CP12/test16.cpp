#include <iostream>
#include <memory>

using std::unique_ptr;

int main()
{
	unique_ptr<int> p(new int(42));
	unique_ptr<int> p1 = new int;
}
