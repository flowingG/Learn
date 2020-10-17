#include <iostream>
#include <string>
using namespace std;
struct inflatable{
	string name;
	int a;
	double b;
};

int main(){
	//using namespace std;
	inflatable a = {
		"aaa", 	1, 2.2
	};
	inflatable b, c;
	b = {"bbb", 2, 3.3};
	c.name = "ccc";
	c.a = 3;
	c.b = 4.4;
	cout << a.name << a.a << a.b << b.name << b.a << b.b << c.name << c.a << c.b << endl;
	return 0;
}
