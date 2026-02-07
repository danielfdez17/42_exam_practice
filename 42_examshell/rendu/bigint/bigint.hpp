#pragma once

#include <iostream>
#include <string>
using namespace std;

class bigint
{
private:
	string	str;
	// size_t	size;
public:
	bigint();
	~bigint();
	bigint(unsigned int);
	bigint(bigint const& obj);
	bigint &operator=(bigint const& obj);

	bigint operator+(bigint const& obj) const;
	bigint operator+=(bigint const& obj);

	bigint operator++(); // pre
	bigint operator++(int); // post

	bigint operator<<(int n) const;
	bigint operator<<(bigint const& obj) const;
	bigint operator>>(int n) const;
	bigint operator>>(bigint const& obj) const;
	bigint operator<<=(int n);
	bigint operator>>=(bigint const& obj);

	bool operator<(bigint const& obj) const;
	bool operator>(bigint const& obj) const;
	bool operator==(bigint const& obj) const;
	bool operator!=(bigint const& obj) const;
	bool operator<=(bigint const& obj) const;
	bool operator>=(bigint const& obj) const;

	string getStr() const;
	
};

ostream &operator<<(ostream &out, bigint const& obj);