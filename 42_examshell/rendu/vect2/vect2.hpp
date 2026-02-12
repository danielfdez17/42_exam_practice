#pragma once

#include <iostream>
using namespace std;

class vect2
{
private:
	int x;
	int y;
public:
	vect2();
	vect2(vect2 const&obj);
	vect2 operator=(vect2 const&obj);
	~vect2();
	vect2(int x, int y);

	int &operator[](int idx);
	int operator[](int idx) const;
	
	vect2 operator++();
	vect2 operator++(int);
	vect2 operator--();
	vect2 operator--(int);

	vect2 operator+=(vect2 const&obj);
	vect2 operator-=(vect2 const&obj);
	vect2 operator*=(vect2 const&obj);
	vect2 operator*=(int n);
	vect2 operator*(int n) const;
	
	vect2 operator+(vect2 const&obj) const;
	vect2 operator-(vect2 const&obj) const;
	vect2 operator-();
	
	bool operator==(vect2 const&obj) const;
	bool operator!=(vect2 const&obj) const;

};

ostream &operator<<(ostream&out, vect2 const& v1);
vect2 operator*(int n, vect2 const &obj);