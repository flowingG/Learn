#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int count = 1;

class numbered{
public:
	numbered(){ mysn = count++; }
	numbered(const numbered&){ mysn = count++; }
	friend void f(const numbered& s);
private:
	int mysn;
};

void f(const numbered& s){ cout << s.mysn << endl; }

int main()
{
	numbered a;
	numbered b = a;
	numbered c = b;
	f(a); f(b); f(c);
	return 0;
}
