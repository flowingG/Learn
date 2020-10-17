#include <iostream>

template<typename T> int compare(const T &a, const T &b)
{
	if(a > b) return 1;
	else return 0;
}

int main()
{
	int a = 5, b = 4;
	std::cout << compare(a, b) << std::endl;
	return 0;
}
