#pragma once

#include "searchable_bag.hpp"

class set : public searchable_bag
{
private:
	searchable_bag &mybag;
public:
	set() = delete;
	set(searchable_bag &bag);
	set(const set &) = delete;
	set &operator=(const set &other) = delete;
	~set();
	virtual void insert (int value);
	virtual void insert (int *data, int size);
	virtual void print() const;
	virtual void clear();
	virtual bool has(int value) const;
	searchable_bag &get_bag() const;
};
