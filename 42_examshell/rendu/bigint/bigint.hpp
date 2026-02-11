#pragma once

#include <iostream>
#include <sstream>
using namespace std;

class bigint
{
private:
	string str;
public:
	bigint();
	~bigint();
	bigint(unsigned int n);
	bigint(const bigint &obj);
	bigint operator=(const bigint &obj);

	bigint operator+(const bigint &obj) const;
	bigint operator+=(const bigint &obj);

	bigint operator++();
	bigint operator++(int);

	bigint operator<<(int n) const;
	bigint operator<<=(int n);
	bigint operator>>(int n) const;
	bigint operator>>=(int n);

	bigint operator<<(bigint const& obj) const;
	bigint operator<<=(bigint const& obj);
	bigint operator>>(bigint const& obj) const;
	bigint operator>>=(bigint const& obj);

	bool operator==(bigint const&obj) const;
	bool operator!=(bigint const&obj) const;
	bool operator<(bigint const&obj) const;
	bool operator<=(bigint const&obj) const;
	bool operator>(bigint const&obj) const;
	bool operator>=(bigint const&obj) const;

	string getStr() const;
	
};

static inline string rotateStr(string s)
{
	return s;
}

static inline char doSum(char a, char b, int &carry)
{

}

static inline string addition(string a, string b)
{
	string ret = "";

	return rotateStr(ret);
}

ostream &operator<<(ostream&out, const bigint &obj);
