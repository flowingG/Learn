#ifndef __STRBLOB_H
#define __STRBLOB_H

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <list>
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::shared_ptr;
using std::list;
using std::make_shared;
using std::out_of_range;
using std::weak_ptr;

class StrBlob{
public:
	friend class StrBlobPtr;
	StrBlobPtr begin() { return StrBlobPtr(*this); }
	StrBlobPtr end() { auto ret = StrBlobPtr(*this, data->size()); return ret; }
	typedef vector<string>::size_type size_type;
	StrBlob();
	StrBlob(std::initializer_list<string> il);
	size_type size() const {return data->size(); }
	bool empty() const {return data->empty(); }
	//增删元素
	void push_back(const string &t) {data->push_back(t); }
	void pop_back();
	//获得元素
	string &front();
	string &back();
private:
	shared_ptr<vector<string>> data;
	void check(size_type i, const string &msg) const;
};

class StrBlobPtr{
public:
	StrBlobPtr():curr(0){ }
	StrBlobPtr(StrBlob &a, size_t sz = 0): wptr(a.data), curr(sz){ } 
	string& deref() const;
	StrBlobPtr& incr();
private:
	shared_ptr<vector<string>> check(std::size_t, const string &) const;
	weak_ptr<vector<string>> wptr;
	std::size_t curr;
};

/***    	StrBlob		***/
//构造函数
StrBlob::StrBlob():data(make_shared<vector<string>>()){ }
StrBlob::StrBlob(std::initializer_list<string> il):data(make_shared<vector<string>>(il)){ }
//成员函数
void StrBlob::check(size_type i, const string &msg) const
{
	if(i >= data->size())
		throw out_of_range(msg);
}
string &StrBlob::front()
{
	check(0, "front on empty StrBlob");
	return data->front();
}
string &StrBlob::back()
{
	check(0, "back on empty StrBlob");
	return data->back();
}
void StrBlob::pop_back()
{
	check(0, "pop_check on empty StrBlob");
	data->pop_back();
}

/***	StrBlobPtr	***/
shared_ptr<vector<string>> StrBlobPtr::check(std::size_t i, const string &msg) const
{
	auto ret = wptr.lock();
	if(!ret)
		throw std::runtime_error("unbound StrBlobPtr");
	if(i >= ret->size())
		throw std::out_of_range(msg);
	return ret;
}
string &StrBlobPtr::deref() const
{
	auto p = check(curr, "dereference past end");
	return (*p)[curr];
}
StrBlobPtr& StrBlobPtr::incr()
{
	check(curr, "increment past end of StrBlobPtr");
	++curr;
	return *this;
}

#endif
