#include <iostream>

class IfThen{
public:
	int operator()(const bool a, int b, int c)
	{
		if(a)
			return b;
		return c;
	}
};

int main()
{
	IfThen it;
	int a, b;
	std::cin >> a >> b;
	std::cout << it((a>b), a, b);
	return 0;
}
