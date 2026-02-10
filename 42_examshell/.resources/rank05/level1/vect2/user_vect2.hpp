#pragma once

#include <iostream>
using namespace std;

class vect2
{
private:
	int		x;
	int		y;
public:
	vect2();
	vect2(int x, int y);
	vect2(vect2 const& copy);
	vect2 operator=(vect2 const& copy);
	~vect2();

	int	operator[](int index) const;
	int &operator[](int index);
	
	vect2 operator++(int); // post
	vect2 operator++(); // pre
	vect2 operator--(int); // post
	vect2 operator--(); // pre
	
	vect2 operator+=(vect2 const& copy);
	vect2 operator-=(vect2 const& copy);

	vect2 operator+(vect2 const& copy) const;
	vect2 operator*(int n) const;
	
	vect2 operator*=(int n);
	
	vect2 operator-(vect2 const& copy) const;

	vect2 operator-() const;

	bool operator==(vect2 const& copy) const;
	bool operator!=(vect2 const& copy) const;
};

ostream &operator<<(ostream &out, vect2 const& obj);
// vect2	operator*(vect2 const& obj, int n);
vect2	operator*(int n, vect2 const& obj);