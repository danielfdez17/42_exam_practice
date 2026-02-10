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
	~vect2();
	vect2(int x, int y);
	vect2(vect2 const&copy);
	vect2 &operator=(vect2 const&copy);

	int &operator[](int idx);
	int operator[](int idx) const;

	// pre
	vect2 operator++();
	// post
	vect2 operator++(int);

	// pre
	vect2 operator--();
	// post
	vect2 operator--(int);

	vect2 operator+(vect2 const&copy) const;
	vect2 operator-(vect2 const&copy) const;
	
	vect2 operator+=(vect2 const&copy);
	vect2 operator-=(vect2 const&copy);
	
	vect2 operator*(int n) const;
	vect2 operator*=(int n);
	vect2 operator*=(vect2 const&copy);

	vect2 operator-();

	bool operator==(vect2 const&copy) const;
	bool operator!=(vect2 const&copy) const;

	
};

ostream &operator<<(ostream&out, vect2 const&o);
vect2	operator*(int n, vect2 const&o);
