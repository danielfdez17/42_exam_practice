#ifndef BIGINT
#define BIGINT

#include <sstream>
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class bigint
{
	private:
		std::string str;
	public:

	bigint();
	~bigint();
	bigint(unsigned int n);
	bigint(bigint const& copy);
	bigint operator=(bigint const& copy);

	bigint operator+(bigint const& copy) const;
	bigint operator+=(bigint const& copy);

	bigint operator++(); // pre
	bigint operator++(int); // pos

	bigint operator<<(int n) const;
	bigint operator>>(int n) const;
	bigint operator<<=(int n);
	bigint operator>>=(int n);

	bool operator==(bigint const& copy) const;
	bool operator!=(bigint const& copy) const;
	bool operator<(bigint const& copy) const;
	bool operator>(bigint const& copy) const;
	bool operator>=(bigint const& copy) const;
	bool operator<=(bigint const& copy) const;
	
	bigint operator<<(bigint const& copy) const;
	bigint operator>>(bigint const& copy) const;

	bigint operator<<=(bigint const& copy);
	bigint operator>>=(bigint const& copy);
	string getStr() const;
};

ostream &operator<<(ostream & out, bigint const& obj);

static inline string rotate(string s)
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

static inline char do_sum(int a, int b, int &carry)
{
	int sum = a + b + carry;
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
	string ret = "";
	size_t left_size = left.size();
	size_t right_size = right.size();

	int carry = 0;
	char c;
	
	while (left_size > 0 && right_size > 0)
	{
		left_size--;
		right_size--;
		c = do_sum(left[left_size] - '0', right[right_size] - '0', carry);
		ret.push_back(c);
	}
	
	while (left_size > 0)
	{
		left_size--;
		// right_size--;
		c = do_sum(left[left_size] - '0', 0, carry);
		ret.push_back(c);
	}

	while (right_size > 0)
	{
		// left_size--;
		right_size--;
		c = do_sum(right[right_size] - '0', 0, carry);
		ret.push_back(c);
	}
	
	return rotate(ret);
}

static inline unsigned int toULong(string s)
{
	unsigned int ret;
	stringstream ss(s);
	ss >> ret;
	return ret;
}


#endif
