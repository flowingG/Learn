#ifndef __HASPTR_H
#define __HASPTR_H

#include <iostream>
#include <string>

using std::string;

class HasPtr{
public:
	HasPtr(const string &s = string()):ps(new string(s)), i(0), use(new std::size_t(1)){ }
	HasPtr(const HasPtr &p):ps(new string(*p.ps)), i(p.i), use(p.use) { ++*use; }
	HasPtr& operator=(const HasPtr &);
	~HasPtr();
private:
	string *ps;
	int i;
	std::size_t *use;
};

HasPtr::~HasPtr()
{
	if(--*use == 0)
	{
		delete ps;
		delete use;
	}
}

HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
	++*rhs.use;
	if(--*use == 0)
	{
		delete ps;
		delete use;
	}
	ps = rhs.ps;
	i = rhs.i;
	use = rhs.use;
	return *this;
}

#endif
