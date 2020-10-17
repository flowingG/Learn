#ifndef __PERSON_H
#define __PERSON_H

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::istream;
using std::ostream;

class Person
{
friend istream &read(istream&, Person&);
friend ostream &print(ostream&, const Person&);
private:
	string name = " ";
	string address = " ";
public:
	Person() = default;
	Person(string a):name(a){}
	Person(string a, string b):name(a), address(b){}
	Person(istream &);

	string getName() const{return name;}
	string getAdd() const{return address;}
};

istream &read(istream&, Person&);
ostream &print(ostream &, const Person&);

Person::Person(istream &is)
{
	read(is, *this);
}

istream &read(istream &is, Person &p)
{
	is >> p.name >> p.address;
	return is;
}

ostream &print(ostream &os, const Person &p)
{
	os << p.getName() << " " << p.getAdd() << endl;
}

#endif
