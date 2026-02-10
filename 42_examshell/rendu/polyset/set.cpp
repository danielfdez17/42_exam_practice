#include "set.hpp"

set::set() : my_bag(nullptr)
{

}
set::set(set &copy)
{
	this->my_bag = copy.my_bag;
}
set::set(searchable_bag &copy)
{
	*this->my_bag = copy;
}
set &set::operator=(const set &copy)
{
	*this->my_bag = copy;
}
set::~set()
{
	if (this->my_bag != nullptr)
	{
		this->my_bag->clear();
	}
}

bool set::has(int value) const
{
	if (this->my_bag != nullptr)
	{
		return this->my_bag->has(value);
	}
	return false;
}
searchable_tree_bag set::convertToAST() const
{
	searchable_tree_bag *ret = dynamic_cast<searchable_tree_bag *>(this->my_bag);
	if (ret)
		return *ret;
	searchable_array_bag *arr = dynamic_cast<searchable_array_bag *>(this->my_bag);
	if (arr)
	{
		searchable_tree_bag tree;
		tree.insert(arr->getData(), arr->getSize());
		return tree;
	}
	return searchable_tree_bag();
}
searchable_tree_bag set::get_bag()
{
	return this->convertToAST();
}
