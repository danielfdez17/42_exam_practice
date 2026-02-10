#pragma once

#include "searchable_bag.hpp"
#include "tree_bag.hpp"

class searchable_tree_bag : virtual public tree_bag, virtual public searchable_bag {
private:
	bool search_aux(node *root, int value) const;
public:
	searchable_tree_bag();
	searchable_tree_bag(const searchable_tree_bag &);
	searchable_tree_bag  &operator=(const searchable_tree_bag &);
	~searchable_tree_bag();
	bool has(int) const;
	void insert(int *array, int size);
};
