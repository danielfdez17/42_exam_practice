#include "user_vect2.hpp"

using namespace std;

vect2::vect2() : x(0), y(0)
{

}
vect2::vect2(int x, int y) : x(x), y(y)
{

}
vect2::vect2(vect2 const& copy)
{
	if (this != &copy)
	{
		this->x = copy.x;
		this->y = copy.y;
	}
}
vect2 vect2::operator=(vect2 const& copy)
{
	if (this != &copy)
	{
		this->x = copy.x;
		this->y = copy.y;
	}
	return *this;
}
vect2::~vect2()
{

}

int	vect2::operator[](int index) const
{
	return index == 0 ? x : y;
}
int &vect2::operator[](int index)
{
	return index == 0 ? x : y;
}

vect2 vect2::operator++(int)
{
	vect2 tmp = *this;
	this->x++;
	this->y++;
	return tmp;
} // post
vect2 vect2::operator++()
{
	this->x++;
	this->y++;
	return *this;
} // pre
vect2 vect2::operator--(int)
{
	vect2 tmp = *this;
	this->x--;
	this->y--;
	return tmp;
} // post
vect2 vect2::operator--()
{
	this->x--;
	this->y--;
	return *this;
} // pre

vect2 vect2::operator+=(vect2 const& copy)
{
	this->x += copy.x;
	this->y += copy.y;
	return *this;
}
vect2 vect2::operator-=(vect2 const& copy)
{
	this->x -= copy.x;
	this->y -= copy.y;
	return *this;
}

vect2 vect2::operator+(vect2 const& copy) const
{
	vect2 tmp = *this;
	tmp.x += copy.x;
	tmp.y += copy.y;
	return tmp;
}
vect2 vect2::operator*(int n) const
{
	vect2 tmp = *this;
	tmp.x *= n;
	tmp.y *= n;
	return tmp;
}

vect2 vect2::operator*=(int n)
{
	this->x *= n;
	this->y *= n;
	return *this;
}

vect2 vect2::operator-(vect2 const& copy) const
{
	vect2 tmp = *this;
	tmp.x -= copy.x;
	tmp.y -= copy.y;
	return tmp;
}

vect2 vect2::operator-() const
{
	vect2 tmp = *this;
	tmp.x = -tmp.x;
	tmp.y = -tmp.y;
	return tmp;
}

bool vect2::operator==(vect2 const& copy) const
{
	return this->x == copy.x && this->y == copy.y;
}
bool vect2::operator!=(vect2 const& copy) const
{
	return this->x != copy.x && this->y != copy.y;
}

ostream &operator<<(ostream &out, vect2 const& v)
{
	return out << "{" << v[0] << ", " << v[1] << "}";
}
// vect2	operator*(vect2 const& obj, int n)
// {
// 	return vect2(obj[0] * n, obj[1] * n);
// }
vect2	operator*(int n, vect2 const& obj)
{
	return vect2(obj[0] * n, obj[1] * n);
}