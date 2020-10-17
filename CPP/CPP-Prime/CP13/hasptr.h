#ifndef __HASPTR_H
#define __HASPTR_H

#include <iostream>
#include <string>

using std::string;

class HasPtr{
public:
	friend void swap(HasPtr&, HasPtr&);
	HasPtr(const string &s = string()):ps(new string(s)), i(0){ }
	HasPtr(const HasPtr &p):ps(new string(*p.ps)), i(p.i){ }
	HasPtr& operator=(const HasPtr &);
	~HasPtr() { delete ps; }
private:
	string *ps;
	int i;
};

inline void swap(HasPtr &rhs, HasPtr &rhs)
{
	using std::swap;
	swap(lhs.ps, rhs.ps);
	swap(lhs.i, rhs.i);
}

HasPtr& HasPtr::operator=(const HasPtr &rhs)
{
	auto newp = new string(*rhs.ps);
	delete ps;
	ps = newp;
	i = rhs.i;
	return *this;
}

#endif
