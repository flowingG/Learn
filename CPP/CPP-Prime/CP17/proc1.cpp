#include <iostream>
#include <regex>
#include <string>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::regex;
using std::smatch;
using std::regex_error;

int main()
{
	try{
		string pattern("[^c]ei");
		pattern = "[a-z]*" + pattern + "[a-z]*";
		cout << pattern << endl;
		regex r(pattern);
		smatch results;
		string test_str = "aei receipt friend theif receive";
		cout << test_str << endl;
		if(regex_search(test_str, results, r))
			cout << results.str() << endl;
	}catch(regex_error e)
	{
		cout << e.what() << "\ncode: " << e.code() << endl;
	}
	return 0;
}
