#include "bigint.hpp"
#include <sstream>

static inline string rotate(string s)
{
	size_t start = 0, end = s.size() - 1;
	while (start < end)
	{
		char c = s[start];
		s[start] = s[end];
		s[end] = c;
		start++;
		end--;
	}
	return s;
}

static inline char do_sum(int a, int b, int& carry)
{
	int sum = a + b + carry;
	if (sum > 9)
		carry = 1;
	else
		carry = 0;
	if (sum <= 9)
		return sum + '0';
	return (sum % 10) + '0';
}

static inline string addition(string left, string right)
{
	string ret = "";
	size_t	left_size = left.size();
	size_t	right_size = right.size();
	int carry = 0;
	char c;
	
	while (left_size > 0 && right_size > 0)
	{
		c = do_sum(left[--left_size] - '0', right[--right_size] - '0', carry);
		ret.push_back(c);
	}

	while (left_size > 0)
	{
		c = do_sum(left[--left_size] - '0', 0, carry);
		ret.push_back(c);
	}

	while (right_size > 0)
	{
		c = do_sum(right[--right_size] - '0', 0, carry);
		ret.push_back(c);
	}
	return rotate(ret);
}

static unsigned long toULong(string s)
{
	unsigned long ret;
	stringstream ss(s);
	ss >> ret;
	return ret;
}

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
bigint &bigint::operator=(bigint const& copy)
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
	tmp.str = addition(this->str, copy.str);
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
} // post

bigint bigint::operator<<(int n) const
{
	bigint tmp = *this;
	if (tmp.str == "0")
		tmp.str = "1";
	while (n--)
	{
		tmp.str.push_back('0');
	}
	return tmp;
}
bigint bigint::operator<<(bigint const& copy) const
{
	bigint tmp = (*this) << toULong(copy.str);
	return tmp;
}

bigint bigint::operator>>(int n) const
{
	bigint tmp = *this;
	if (n > this->str.size())
		tmp.str = "0";
	else
		tmp.str.erase(tmp.str.size() - n, n);
	return tmp;
}
bigint bigint::operator>>(bigint const& copy) const
{
	bigint tmp = (*this) >> toULong(copy.str);
	return tmp;
}

bigint bigint::operator<<=(int n)
{
	if (this->str == "0")
		this->str = "1";
	while (n--)
	{
		this->str.push_back('0');
	}
	return *this;
}

bigint bigint::operator>>=(bigint const& copy)
{
	*this = *this >> toULong(copy.str);
	return *this;
}

bool bigint::operator<(bigint const& copy) const
{
	return toULong(this->str) < toULong(copy.str);
	// if (this->str.size() < copy.str.size())
	// 	return true;
	// if (this->str.size() == copy.str.size())
	// 	return this->str == copy.str;
	// return false;
}
bool bigint::operator>(bigint const& copy) const
{
	return toULong(this->str) > toULong(copy.str);
	// if (this->str.size() > copy.str.size())
	// 	return true;
	// if (this->str.size() == copy.str.size())
	// 	return this->str == copy.str;
	// return false;
}
bool bigint::operator==(bigint const& copy) const
{
	return toULong(this->str) == toULong(copy.str);
	// if (this->str.size() != copy.str.size())
	// 	return false;
	// return this->str == copy.str;
}
bool bigint::operator!=(bigint const& copy) const
{
	return toULong(this->str) != toULong(copy.str);
	// if (this->str.size() != copy.str.size())
	// 	return true;
	// return this->str != copy.str;
}
bool bigint::operator<=(bigint const& copy) const
{
	return toULong(this->str) <= toULong(copy.str);
	// if (this->str.size() < copy.str.size())
	// 	return true;
	// if (this->str.size() == copy.str.size())
	// 	return this->str == copy.str;
	// return false;
}
bool bigint::operator>=(bigint const& copy) const
{
	return toULong(this->str) >= toULong(copy.str);
	// if (this->str.size() > copy.str.size())
	// 	return true;
	// if (this->str.size() == copy.str.size())
	// 	return this->str == copy.str;
	// return false;
}

string bigint::getStr() const
{
	return this->str;
}

ostream &operator<<(ostream &out, bigint const& copy)
{
	return out << copy.getStr();
}