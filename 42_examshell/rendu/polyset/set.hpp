#pragma once

#include "searchable_bag.hpp"
#include "searchable_tree_bag.hpp"
#include "searchable_array_bag.hpp"

class set : virtual public searchable_bag {
private:
	searchable_bag *my_bag;
	searchable_tree_bag convertToAST() const;
public:
	set();
	set(set &copy);
	set(searchable_bag &);
	set &operator=(const set &copy);
	~set();
	bool has(int) const;
	searchable_tree_bag get_bag();
};
