#include "bigint.hpp"

bigint::bigint()
{

}
bigint::~bigint()
{

}
bigint::bigint(unsigned int n)
{

}
bigint::bigint(const bigint &obj)
{

}
bigint bigint::operator=(const bigint &obj)
{

}

bigint bigint::operator+(const bigint &obj) const
{

}
bigint bigint::operator+=(const bigint &obj)
{

}

bigint bigint::operator++()
{

}
bigint bigint::operator++(int)
{

}

bigint bigint::operator<<(int n) const
{

}
bigint bigint::operator<<=(int n)
{

}
bigint bigint::operator>>(int n) const
{

}
bigint bigint::operator>>=(int n)
{

}

bigint bigint::operator<<(bigint const& obj) const
{

}
bigint bigint::operator<<=(bigint const& obj)
{

}
bigint bigint::operator>>(bigint const& obj) const
{

}
bigint bigint::operator>>=(bigint const& obj)
{

}

bool bigint::operator==(bigint const&obj) const
{

}
bool bigint::operator!=(bigint const&obj) const
{

}
bool bigint::operator<(bigint const&obj) const
{

}
bool bigint::operator<=(bigint const&obj) const
{

}
bool bigint::operator>(bigint const&obj) const
{

}
bool bigint::operator>=(bigint const&obj) const
{

}

string bigint::getStr() const
{
	return this->str;
}

ostream &operator<<(ostream&out, const bigint &obj)
{
	return out << obj.getStr();
}
