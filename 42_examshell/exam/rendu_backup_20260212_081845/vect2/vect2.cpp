#include "vect2.hpp"

vect2::vect2() : x(0), y(0)
{

}
vect2::vect2(vect2 const&obj)
{
	if (this != &obj)
	{
		this->x = obj.x;
		this->y = obj.y;
	}
}
vect2 vect2::operator=(vect2 const&obj)
{
	if (this != &obj)
	{
		this->x = obj.x;
		this->y = obj.y;
	}
	return *this;
}
vect2::~vect2()
{

}
vect2::vect2(int x, int y) : x(x), y(y)
{

}

int &vect2::operator[](int idx)
{
	return idx == 0 ? this->x : this->y;
}
int vect2::operator[](int idx) const
{
	return idx == 0 ? this->x : this->y;
}

vect2 vect2::operator++()
{
	this->x++;
	this->y++;
	return *this;
}
vect2 vect2::operator++(int)
{
	vect2 tmp = *this;
	this->x++;
	this->y++;
	return tmp;
}
vect2 vect2::operator--()
{
	this->x--;
	this->y--;
	return *this;
}
vect2 vect2::operator--(int)
{
	vect2 tmp = *this;
	this->x--;
	this->y--;
	return tmp;
}

vect2 vect2::operator+=(vect2 const&obj)
{
	this->x += obj.x;
	this->y += obj.y;
	return *this;
}
vect2 vect2::operator-=(vect2 const&obj)
{
	this->x -= obj.x;
	this->y -= obj.y;
	return *this;
}
vect2 vect2::operator*=(vect2 const&obj)
{
	this->x *= obj.x;
	this->y *= obj.y;
	return *this;
}
vect2 vect2::operator*=(int n)
{
	this->x *= n;
	this->y *= n;
	return *this;
}
vect2 vect2::operator*(int n) const
{
	return vect2(this->x * n, this->y * n);
}

vect2 vect2::operator+(vect2 const&obj) const
{
	return vect2(this->x + obj.x, this->y + obj.y);
}
vect2 vect2::operator-(vect2 const&obj) const
{
	return vect2(this->x - obj.x, this->y - obj.y);
}
vect2 vect2::operator-()
{
	this->x = -this->x;
	this->y = -this->y;
	return *this;
}

bool vect2::operator==(vect2 const&obj) const
{
	return this->x == obj.x && this->y == obj.y;
}
bool vect2::operator!=(vect2 const&obj) const
{
	return this->x != obj.x || this->y != obj.y;
}

ostream &operator<<(ostream&out, vect2 const& v1)
{
	return out << "{" << v1[0] << ", " << v1[1] << "}";
}
vect2 operator*(int n, vect2 const &obj)
{
	return vect2(obj[0] * n, obj[1] * n);
}