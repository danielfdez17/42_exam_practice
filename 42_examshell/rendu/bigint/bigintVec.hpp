#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>
using namespace std;

class bigintVec
{
private:
	list<int>v;
public:
	bigintVec();
	~bigintVec();
	bigintVec(unsigned int);
	bigintVec(bigintVec const& obj);
	bigintVec &operator=(bigintVec const& obj);

	bigintVec operator+(bigintVec const& obj) const;
	bigintVec operator+=(bigintVec const& obj);

	bigintVec operator++(); // pre
	bigintVec operator++(int); // post

	bigintVec operator<<(int n) const;
	bigintVec operator<<(bigintVec const& obj) const;
	bigintVec operator>>(int n) const;
	bigintVec operator>>(bigintVec const& obj) const;
	bigintVec operator<<=(int n);
	bigintVec operator>>=(bigintVec const& obj);

	bool operator<(bigintVec const& obj) const;
	bool operator>(bigintVec const& obj) const;
	bool operator==(bigintVec const& obj) const;
	bool operator!=(bigintVec const& obj) const;
	bool operator<=(bigintVec const& obj) const;
	bool operator>=(bigintVec const& obj) const;

	string getStr() const;
	
};

ostream &operator<<(ostream &out, bigintVec const& obj);