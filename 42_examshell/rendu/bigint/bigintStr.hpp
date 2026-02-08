#pragma once

#include <iostream>
#include <string>
using namespace std;

class bigintStr
{
private:
	string	str;
	// size_t	size;
public:
	bigintStr();
	~bigintStr();
	bigintStr(unsigned int);
	bigintStr(bigintStr const& obj);
	bigintStr &operator=(bigintStr const& obj);

	bigintStr operator+(bigintStr const& obj) const;
	bigintStr operator+=(bigintStr const& obj);

	bigintStr operator++(); // pre
	bigintStr operator++(int); // post

	bigintStr operator<<(int n) const;
	bigintStr operator<<(bigintStr const& obj) const;
	bigintStr operator>>(int n) const;
	bigintStr operator>>(bigintStr const& obj) const;
	bigintStr operator<<=(int n);
	bigintStr operator>>=(bigintStr const& obj);

	bool operator<(bigintStr const& obj) const;
	bool operator>(bigintStr const& obj) const;
	bool operator==(bigintStr const& obj) const;
	bool operator!=(bigintStr const& obj) const;
	bool operator<=(bigintStr const& obj) const;
	bool operator>=(bigintStr const& obj) const;

	string getStr() const;
	
};

ostream &operator<<(ostream &out, bigintStr const& obj);