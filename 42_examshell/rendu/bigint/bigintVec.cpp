#include "bigintVec.hpp"

bigintVec::bigintVec() : v(1, 0) {}
bigintVec::~bigintVec() {}
bigintVec::bigintVec(unsigned int n)
{
	if (n == 0)
	{
		this->v = list<int>(1, 0);
		return ;
	}
	while (n != 0)
	{
		this->v.push_front(n % 10);
	}
}
