#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() : tree_bag()
{
	this->tree = nullptr;
}
searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &copy) : tree_bag(copy)
{
	this->tree = copy_node(copy.tree);
}
searchable_tree_bag::~searchable_tree_bag()
{
	destroy_tree(this->tree);
}
searchable_tree_bag  &searchable_tree_bag::operator=(const searchable_tree_bag &copy)
{
	if (this != &copy)
	{
		destroy_tree(this->tree);
		this->tree = copy_node(this->tree);
	}
	return *this;
}

bool	searchable_tree_bag::search_aux(node *root, int value) const
{
	if (!root)
		return false;
	if (root->value == value)
		return true;
	if (value < root->value)
		return search_aux(root->l, value);
	return search_aux(root->r, value);
}

bool	searchable_tree_bag::has(int value) const
{
	return this->search_aux(this->tree, value);
}
