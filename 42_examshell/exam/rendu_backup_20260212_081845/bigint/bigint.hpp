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

	bigint operator<<(unsigned long long n) const;
	bigint operator<<=(unsigned long long n);
	bigint operator>>(unsigned long long n) const;
	bigint operator>>=(unsigned long long n);

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

static inline unsigned long long toULL(string s)
{
	stringstream ss(s);
	unsigned long long ret;
	ss >> ret;
	return ret;
}

static inline string rotateStr(string s)
{
	size_t start = 0, end = s.size() - 1;
	char c;
	while (start < end)
	{
		c = s[start];
		s[start] = s[end];
		s[end] = c;
		start++;
		end--;
	}
	return s;
}

static inline char doSum(char a, char b, int &carry)
{
	int ia = a - '0', ib = b - '0';
	int sum = ia + ib + carry;
	if (sum > 9)
		carry = 1;
	else
		carry = 0;
	if (sum > 9)
		return (sum % 10) + '0';
	return sum + '0';
}

static inline string addition(string a, string b)
{
	string ret = "";
	size_t a_size = a.size();
	size_t b_size = b.size();
	int carry = 0;
	char c;
	while (a_size > 0 && b_size > 0)
	{
		a_size--;
		b_size--;
		c = doSum(a[a_size], b[b_size], carry);
		ret.push_back(c);
	}
	while (a_size > 0)
	{
		a_size--;
		c = doSum(a[a_size], '0', carry);
		ret.push_back(c);
	}
	while (b_size > 0)
	{
		b_size--;
		c = doSum(b[b_size], '0', carry);
		ret.push_back(c);
	}
	return rotateStr(ret);
}

ostream &operator<<(ostream&out, const bigint &obj);
