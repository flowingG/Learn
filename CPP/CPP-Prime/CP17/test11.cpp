#include <iostream>
#include <bitset>

using std::cin;
using std::cout;
using std::endl;
using std::bitset;

class Test{
public:
	Test() = default;
	Test(bitset<100> bit) : b(bit) { }
	friend bool isTest(const Test &, int);
private:
	int index;
	bitset<100> b;
};

bool isTest(const Test& test, int index){
	return test.b[index - 1];
}

int main()
{
	bitset<100> b(0xef2ffff);
	cout << b << endl;
	Test t(b);
	int index = 1, count = 0;
	for(int i = 0; i <= 100; i++, index++)
	{	
		if(isTest(t, index))
			count++;
	}
	cout << count << "分" << endl;
	return 0;
}
