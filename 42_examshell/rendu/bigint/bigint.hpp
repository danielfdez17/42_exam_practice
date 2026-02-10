#pragma once
#include<iostream>
#include<string>
#include<sstream>
using namespace std;

class bigint
{
private:
	string str;
public:
	bigint();
	~bigint();
	bigint(unsigned int n);
	bigint(bigint const&copy);
	bigint &operator=(bigint const&copy);

	bigint operator+(bigint const&copy) const;
	bigint operator+=(bigint const&copy);

	bigint operator++(int);
	bigint operator++();

	bigint operator<<(int n) const;
	bigint operator<<=(int n);
	bigint operator<<(bigint const&copy) const;
	bigint operator<<=(bigint const&copy);

	bigint operator>>(int n) const;
	bigint operator>>=(int n);
	bigint operator>>(bigint const&copy) const;
	bigint operator>>=(bigint const&copy);

	bool operator==(bigint const&copy) const;
	bool operator!=(bigint const&copy) const;
	bool operator<(bigint const&copy) const;
	bool operator<=(bigint const&copy) const;
	bool operator>(bigint const&copy) const;
	bool operator>=(bigint const&copy) const;
	
	string getStr() const;
};

static inline unsigned long toUlong(string s)
{
	stringstream ss(s);
	unsigned long ret;
	ss >> ret;
	return ret;
}

static inline string rotateStr(string s)
{
	size_t start = 0, end = s.size() - 1;
	while (start < end)
	{
		char c = s[start];
		s[start] = s[end];
		s[end] = c;
		start++;
		end--;
	}
	return s;
}

static inline char doSum(char a, char b, int &carry)
{
	int ia = a - '0';
	int ib = b - '0';
	int sum = ia + ib + carry;
	if (sum > 9)
		carry = 1;
	else
		carry = 0;
	if (sum > 9)
		return (sum % 10) + '0';
	return sum + '0';
}

static inline string addition(string left, string right)
{
	string ret;
	size_t	left_size = left.size();
	size_t	right_size = right.size();
	int carry = 0;
	char c;
	while (left_size > 0 && right_size > 0)
	{
		left_size--;
		right_size--;
		c = doSum(left[left_size], right[right_size], carry);
		ret.push_back(c);
	}

	while (left_size > 0) // && right_size > 0)
	{
		left_size--;
		// right_size--;
		c = doSum(left[left_size], '0', carry);
		ret.push_back(c);
	}

	while (right_size > 0) // && right_size > 0)
	{
		// left_size--;
		right_size--;
		c = doSum(right[right_size], '0', carry);
		ret.push_back(c);
	}
	return rotateStr(ret);
}

ostream &operator<<(ostream&out, const bigint&o);
