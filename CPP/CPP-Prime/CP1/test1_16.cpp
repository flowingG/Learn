#include <iostream>
int main(){
	int va, sum;
	sum = 0;
	while(std::cin >> va)
		sum += va;
	std::cout << sum << std::endl;
	return 0;
}
