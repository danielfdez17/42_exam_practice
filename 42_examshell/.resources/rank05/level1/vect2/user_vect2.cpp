#include "user_vect2.hpp"

vect2::vect2() : x(0), y(0)
{

}
vect2::~vect2()
{

}
vect2::vect2(int x, int y) : x(x), y(y)
{

}
vect2::vect2(vect2 const&copy)
{
	if (this != &copy)
	{
		this->x = copy.x;
		this->y = copy.y;
	}
}
vect2 &vect2::operator=(vect2 const&copy)
{
	if (this != &copy)
	{
		this->x = copy.x;
		this->y = copy.y;
	}
	return *this;
}

int &vect2::operator[](int idx)
{
	return idx == 0 ? this->x : this->y;
}
int vect2::operator[](int idx) const
{
	return idx == 0 ? this->x : this->y;
}

vect2 vect2::operator++(int)
{
	vect2 copy = *this;
	this->x++;
	this->y++;
	return copy;
}
vect2 vect2::operator++()
{
	this->x++;
	this->y++;
	return *this;
}
vect2 vect2::operator--(int)
{
	vect2 copy = *this;
	this->x--;
	this->y--;
	return copy;
}
vect2 vect2::operator--()
{
	this->x--;
	this->y--;
	return *this;
}

vect2 vect2::operator+=(vect2 const&copy)
{
	this->x += copy.x;
	this->y += copy.y;
	return *this;
}
vect2 vect2::operator-=(vect2 const&copy)
{
	this->x -= copy.x;
	this->y -= copy.y;
	return *this;
}

vect2 vect2::operator+(vect2 const&copy) const
{
	return vect2(this->x + copy.x, this->y + copy.y);
}
vect2 vect2::operator-(vect2 const&copy) const
{
	return vect2(this->x - copy.x, this->y - copy.y);
}
vect2 vect2::operator*(int n) const
{
	return vect2(this->x * n, this->y * n);
}

vect2 vect2::operator*=(int n)
{
	this->x *= n;
	this->y *= n;
	return *this;
}

vect2 vect2::operator-()
{
	this->x *= -1;
	this->y *= -1;
	return *this;
}

bool vect2::operator==(vect2 const&copy) const
{
	return this->x == copy.x && this->y == copy.y;
}
bool vect2::operator!=(vect2 const&copy) const
{
	return this->x != copy.x && this->y != copy.y;
}

ostream &operator<<(ostream&out, vect2 const&v1)
{
	return out << "{" << v1[0] << ", " << v1[1] << "}";
}
vect2 operator*(int n, vect2 const&o)
{
	return vect2(o[0] * n, o[1] * n);
}