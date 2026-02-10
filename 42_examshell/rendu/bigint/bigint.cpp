#include "bigint.hpp"

bigint::bigint() : str("0")
{

}
bigint::~bigint(){}
bigint::bigint(unsigned int n)
{
	stringstream ss;
	ss << n;
	this->str = ss.str();
}
bigint::bigint(bigint const&copy)
{
	if (this != &copy)
	{
		this->str = copy.str;
	}
}
bigint &bigint::operator=(bigint const&copy)
{
	if (this != &copy)
	{
		this->str = copy.str;
	}
	return *this;
}

bigint bigint::operator+(bigint const&copy) const
{
	bigint ret = *this;
	ret.str = addition(ret.str, copy.str);
	return ret;
}

bigint bigint::operator+=(bigint const&copy)
{
	this->str = addition(this->str, copy.str);
	return *this;
}

bigint bigint::operator++(int)
{
	bigint ret = *this;
	this->str = addition(this->str, "1");
	return ret;
}

bigint bigint::operator++()
{
	this->str = addition(this->str, "1");
	return *this;
}

bigint bigint::operator<<(int n) const
{
	if (this->str == "0")
		return *this;
	bigint ret = *this;
	while (n--)
		ret.str.push_back('0');
	return ret;
}

bigint bigint::operator<<=(int n)
{
	if (this->str == "0")
		return *this;
	while (n--)
		this->str.push_back('0');
	return *this;
}

bigint bigint::operator<<(bigint const&copy) const
{
	return *this << (int)toUlong(copy.str);
}
bigint bigint::operator<<=(bigint const&copy)
{
	*this = *this << (int)toUlong(copy.str);
	return *this;
}

bigint bigint::operator>>(int n) const
{
	if (n > this->str.size() || this->str == "0")
	{
		return bigint();
	}
	bigint ret = *this;
	ret.str = ret.str.substr(0, ret.str.size() - n);
	return ret;
}

bigint bigint::operator>>=(int n)
{
	if (n > this->str.size() || this->str == "0")
	{
		return bigint();
	}
	this->str = this->str.substr(0, this->str.size() - n);
	return *this;
}

bigint bigint::operator>>(bigint const&copy) const
{
	return *this >> (int)toUlong(copy.str);
}
bigint bigint::operator>>=(bigint const&copy)
{
	*this = *this >> (int)toUlong(copy.str);
	return *this;
}

bool bigint::operator==(bigint const&copy) const
{
	return toUlong(this->str) == toUlong(copy.str);
}
bool bigint::operator!=(bigint const&copy) const
{
	return toUlong(this->str) != toUlong(copy.str);
}
bool bigint::operator<(bigint const&copy) const
{
	return toUlong(this->str) < toUlong(copy.str);
}
bool bigint::operator<=(bigint const&copy) const
{
	return toUlong(this->str) <= toUlong(copy.str);
}
bool bigint::operator>(bigint const&copy) const
{
	return toUlong(this->str) > toUlong(copy.str);
}
bool bigint::operator>=(bigint const&copy) const
{
	return toUlong(this->str) >= toUlong(copy.str);
}

string bigint::getStr() const
{
	return this->str;
}

ostream &operator<<(ostream&out, const bigint&o)
{
	return out << o.getStr();
}