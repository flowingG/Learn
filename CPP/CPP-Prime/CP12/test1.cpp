#include "strblob.h"

int main()
{
	StrBlob b1;
	{
		StrBlob b2 = {"a", "an", "the"};
		b1 = b2;
		b2.push_back("about");
		cout << b1.front() << " " << b2.front() << endl;
	b1.pop_back();
		
		cout << b1.back() << " " << b2.back() << endl;
	}
	return 0;
}
