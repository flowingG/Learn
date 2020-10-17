#ifndef _SALE_H
#define _SALE_H

#include <iostream>
#include <string>
using std::cin;

//std::istream &read(std::istream &, Sales_data &);

class Sales_data{
//友元
friend Sales_data add(const Sales_data&, const Sales_data&);
friend std::ostream &print(std::ostream&, const Sales_data&);
friend std::istream &read(std::istream &, Sales_data &);
private:
	std::string bookNo;
	unsigned units_sold = 0;
	double revenue = 0.0;
	
public:
	Sales_data(std::string s, unsigned cnt, double price):bookNo(s), units_sold(cnt), revenue(cnt * price){}
	Sales_data(): Sales_data("", 0, 0){}
	Sales_data(std::string s): Sales_data(s, 0, 0){}
	Sales_data(std::istream &is): Sales_data(){ read(is, *this);}
	//关于对象的操作
	std::string isbn() const{return bookNo;}
	Sales_data& combine(const Sales_data&);
	double avg_price() const;
};

Sales_data add(const Sales_data&, const Sales_data&);
std::ostream &print(std::ostream&, const Sales_data&);
std::istream &read(std::istream&, Sales_data&);

/*
Sales_data::Sales_data(std::istream &is)
{
	read(is, *this);
}
*/
inline double Sales_data::avg_price() const
{
	if(units_sold)
		return revenue / units_sold;
	else 
		return 0;
}

Sales_data& Sales_data::combine(const Sales_data &rhs)
{
	units_sold += rhs.units_sold;
	revenue += rhs.revenue;
	return *this;	
}

std::istream &read(std::istream &is, Sales_data &item)
{
	double price = 0;
	is >> item.bookNo >> item.units_sold >> price;
	item.revenue = price * item.units_sold;
	return is;
}

std::ostream &print(std::ostream &os, const Sales_data &item)
{
	os << item.isbn() << " " << item.units_sold << " " << item.revenue << " " << item.avg_price();
	return os;
}

Sales_data add(const Sales_data &lhs, const Sales_data &rhs)
{
	Sales_data sum = lhs;
	sum.combine(rhs);
	return sum;
}

#endif
