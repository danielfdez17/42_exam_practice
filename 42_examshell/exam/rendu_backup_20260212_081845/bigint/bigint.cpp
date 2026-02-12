#include "bigint.hpp"

bigint::bigint() : str("0")
{

}
bigint::~bigint()
{

}
bigint::bigint(unsigned int n)
{
	stringstream ss;
	ss << n;
	this->str = ss.str();
}
bigint::bigint(const bigint &obj)
{
	if (this != &obj)
	{
		this->str = obj.str;
	}
}
bigint bigint::operator=(const bigint &obj)
{
	if (this != &obj)
	{
		this->str = obj.str;
	}
	return *this;
}

bigint bigint::operator+(const bigint &obj) const
{
	bigint ret = *this;
	ret.str = addition(ret.str, obj.str);
	return ret;
}
bigint bigint::operator+=(const bigint &obj)
{
	this->str = addition(this->str, obj.str);
	return *this;
}

bigint bigint::operator++()
{
	this->str = addition(this->str, "1");
	return *this;
}
bigint bigint::operator++(int)
{
	bigint tmp = *this;
	this->str = addition(this->str, "1");
	return tmp;
}

bigint bigint::operator<<(unsigned long long n) const
{
	if (this->str == "0")
		return *this;
	bigint tmp = *this;
	while (n--)
	{
		tmp.str.push_back('0');
	}
	return tmp;
}
bigint bigint::operator<<=(unsigned long long n)
{
	if (this->str == "0")
		return *this;
	while (n--)
	{
		this->str.push_back('0');
	}
	return *this;
}
bigint bigint::operator>>(unsigned long long n) const
{
	if (n > (int)this->str.size())
	{
		return bigint();
	}
	bigint tmp = *this;
	tmp.str = tmp.str.substr(0, tmp.str.size() - 1);
	return tmp;
}
bigint bigint::operator>>=(unsigned long long n)
{
	if (n > (int)this->str.size())
	{
		this->str = "0";
		return *this;
	}
	this->str = this->str.substr(0, this->str.size() - n);
	return *this;
}

bigint bigint::operator<<(bigint const& obj) const
{
	return *this << toULL(obj.str);
}
bigint bigint::operator<<=(bigint const& obj)
{
	*this <<= toULL(obj.str);
	return *this;
}
bigint bigint::operator>>(bigint const& obj) const
{
	return *this >> toULL(obj.str);
}
bigint bigint::operator>>=(bigint const& obj)
{
	*this >>= toULL(obj.str);
	return *this;
}

bool bigint::operator==(bigint const&obj) const
{
	return toULL(this->str) == toULL(obj.str);
}
bool bigint::operator!=(bigint const&obj) const
{
	return toULL(this->str) != toULL(obj.str);
}
bool bigint::operator<(bigint const&obj) const
{
	return toULL(this->str) < toULL(obj.str);
}
bool bigint::operator<=(bigint const&obj) const
{
	return toULL(this->str) <= toULL(obj.str);
}
bool bigint::operator>(bigint const&obj) const
{
	return toULL(this->str) > toULL(obj.str);
}
bool bigint::operator>=(bigint const&obj) const
{
	return toULL(this->str) >= toULL(obj.str);
}

string bigint::getStr() const
{
	return this->str;
}

ostream &operator<<(ostream&out, const bigint &obj)
{
	return out << obj.getStr();
}
