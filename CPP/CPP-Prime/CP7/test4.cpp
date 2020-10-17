#include "person.h"

int main()
{
	Person p1;
	Person p2("a");
	Person p3("b", "?");
	Person p4(cin);
	print(cout, p1);
	print(cout, p2);
	print(cout, p3);
	print(cout, p4);
	return 0;
}
