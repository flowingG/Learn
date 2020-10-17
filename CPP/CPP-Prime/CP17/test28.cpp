#include <iostream>
#include <random>

using std::cin;
using std::cout;
using std::endl;
using std::default_random_engine;
using std::uniform_int_distribution;

unsigned int f1();
unsigned int f1(int seed);

int main()
{
	for(int i = 0; i < 20; ++i)
		cout << f1() << endl;
	cout << "-------------------------------" << endl;	
	for(int i = 0; i < 20; ++i)
	{
		cout << "Seed: " << i << "Random: " << f1(i) << endl;
	}
	return 0;
}

unsigned int f1()
{
	static default_random_engine e;
	static uniform_int_distribution<unsigned> u(0, 100);
	return u(e);
}

unsigned int f1(int seed)
{
	static default_random_engine e(seed);
	static uniform_int_distribution<unsigned> u(0, 100);
	if(seed == 0)
		return e.min();
	if(seed == 19)
		return e.max();
	return u(e);
}
