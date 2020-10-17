#include <iostream>

using std::cin;
using std::cout;
using std::endl;

template<typename T> class First{
public:
	First() = default;
	First(T a, T b):t1(a), t2(b){ }
	T add(){ return t1 + t2; }
private:
	T t1;
	T t2;
};

int main()
{
	First<int> first(1, 2);
	cout << first.add() << endl;
	return 0;
}
