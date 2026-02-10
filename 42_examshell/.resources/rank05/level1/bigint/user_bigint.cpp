#include "user_bigint.hpp"

using namespace std;

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
bigint::bigint(bigint const& copy)
{
	if (this != &copy)
	{
		this->str = copy.str;
	}
}
bigint bigint::operator=(bigint const& copy)
{
	if (this != &copy)
	{
		this->str = copy.str;
	}
	return *this;
}

bigint bigint::operator+(bigint const& copy) const
{
	bigint tmp = *this;
	tmp.str = addition(tmp.str, copy.str);
	return tmp;
}
bigint bigint::operator+=(bigint const& copy)
{
	this->str = addition(this->str, copy.str);
	return *this;
}

bigint bigint::operator++()
{
	this->str = addition(this->str, "1");
	return *this;
} // pre
bigint bigint::operator++(int)
{
	bigint tmp = *this;
	this->str = addition(this->str, "1");
	return tmp;
} // pos

bigint bigint::operator<<(int n) const
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
bigint bigint::operator>>(int n) const
{
	bigint tmp = *this;
	if (this->str == "0" || this->str.size() < (long unsigned int)n)
	{
		tmp.str = "0";
		return tmp;
	}
	tmp.str = tmp.str.substr(0, tmp.str.size() - n);
	return tmp;
}
bigint bigint::operator<<=(int n)
{
	*this = *this << n;
	return *this;
}
bigint bigint::operator>>=(int n)
{
	*this = *this >> n;
	return *this;
}

bool bigint::operator==(bigint const& copy) const
{
	if (this->str.size() != copy.str.size())
		return false;
	for (size_t i = this->str.size() - 1; i > 0; i--)
	{
		if (this->str[i] != copy.str[i])
			return false;
	}
	return true;
}
bool bigint::operator!=(bigint const& copy) const
{
	// if (this->str.size() != copy.str.size())
	// 	return true;
	// for (size_t i = 0; i < this->str.size(); i++)
	// {
	// 	if (this->str[i] == copy.str[i])
	// 		return false;
	// }
	// return true;
	return !(*this == copy);
}
bool bigint::operator<(bigint const& copy) const
{
	// if (this->str.size() < copy.str.size())
	// 	return true;
	// if (this->str.size() > copy.str.size())
	// 	return false;
	// for (size_t i = this->str.size() - 1; i > 0; i--)
	// {
	// 	if (this->str[i] >= copy.str[i])
	// 		return false;
	// }
	// return true;
	return toULong(this->str) < toULong(copy.str);
}
bool bigint::operator>(bigint const& copy) const
{
	// if (this->str.size() > copy.str.size())
	// 	return true;
	// if (this->str.size() < copy.str.size())
	// 	return false;
	// for (size_t i = this->str.size() - 1; i > 0; i--)
	// {
	// 	if (this->str[i] <= copy.str[i])
	// 		return false;
	// }
	// return true;
	return toULong(this->str) > toULong(copy.str);
}
bool bigint::operator>=(bigint const& copy) const
{
	// if (this->str.size() > copy.str.size())
	// 	return true;
	// if (this->str.size() < copy.str.size())
	// 	return false;
	// for (size_t i = this->str.size() - 1; i > 0; i--)
	// {
	// 	if (this->str[i] < copy.str[i])
	// 		return false;
	// }
	// return true;
	return toULong(this->str) >= toULong(copy.str);
}
bool bigint::operator<=(bigint const& copy) const
{
	// if (this->str.size() < copy.str.size())
	// 	return true;
	// if (this->str.size() > copy.str.size())
	// 	return false;
	// for (size_t i = this->str.size() - 1; i > 0; i--)
	// {
	// 	if (this->str[i] > copy.str[i])
	// 		return false;
	// }
	// return true;
	return toULong(this->str) <= toULong(copy.str);
}

bigint bigint::operator<<(bigint const& copy) const
{
	bigint tmp = *this << toULong(copy.str);
	return tmp;
}
bigint bigint::operator>>(bigint const& copy) const
{
	bigint tmp = *this >> toULong(copy.str);
	return tmp;
}

bigint bigint::operator<<=(bigint const& copy)
{
	*this = *this << toULong(copy.str);
	return *this;
}
bigint bigint::operator>>=(bigint const& copy)
{
	*this = *this >> toULong(copy.str);
	return *this;
}
string bigint::getStr() const
{
	return this->str;
}

ostream &operator<<(ostream & out, bigint const& obj)
{
	return out << obj.getStr();
}

