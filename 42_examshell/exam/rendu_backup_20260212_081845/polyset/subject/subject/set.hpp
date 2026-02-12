#pragma once

#include "searchable_bag.hpp"

class set
{
private:
	searchable_bag &mybag;
public:
	set() = delete;
	~set();
	set(set const&src) = delete;
	set operator=(set const& src) = delete;
	set(searchable_bag &bag);

	void insert(int n);
	void insert(int *data, int n);
	bool has(int value) const;
	void print() const;
	void clear();
	const searchable_bag &get_bag();
};
