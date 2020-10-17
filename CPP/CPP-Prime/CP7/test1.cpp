#include "person.h"

int main()
{
	Person p1;
	cin >> p1.name >> p1.address;
	cout << p1.getName() << p1.getAdd() << endl;
	return 0;
}
