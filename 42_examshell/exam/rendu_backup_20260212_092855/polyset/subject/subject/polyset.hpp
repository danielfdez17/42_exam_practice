#pragma once

#include "array_bag.hpp"
#include "searchable_bag.hpp"
#include "tree_bag.hpp"

class searchable_array_bag : public array_bag, public searchable_bag
{
private:
public:
	searchable_array_bag();
	searchable_array_bag(const searchable_array_bag &);
	searchable_array_bag &operator=(const searchable_array_bag &other);
	~searchable_array_bag();

	bool has(int value) const;
	
};

class searchable_tree_bag : public tree_bag, public searchable_bag
{
private:
	bool searchAux(node *root, int value) const;
public:
	searchable_tree_bag();
	searchable_tree_bag(const searchable_tree_bag &);
	searchable_tree_bag &operator=(const searchable_tree_bag &other);
	~searchable_tree_bag();

	bool has(int value) const;
	
};

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
