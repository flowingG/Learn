#include <iostream>
#include <string>
#include <vector>

using std::string;

template<typename T, unsigned a> T* begin_my(T (&arr)[a])
{
	return arr;
}

template<typename T, unsigned a> T* end_my(T (&arr)[a])
{
	return arr + a;
}

int main()
{
	string s[] = {"aaa", "bbb", "ccc"};
	std::cout << *(begin_my(s) + 1) << std::endl;
	std::cout << *(end_my(s) - 1) << std::endl;
	return 0;
}
