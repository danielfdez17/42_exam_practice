#include "searchable_tree_bag.hpp"

searchable_tree_bag::searchable_tree_bag() : tree_bag()
{
	this->tree = nullptr;
}
searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &src) : tree_bag(src)
{
	tree = copy_node(src.tree);
}
searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &src)
{
	if (this != &src) {
		destroy_tree(tree);
		tree = copy_node(src.tree);
	}
	return *this;
}
searchable_tree_bag::~searchable_tree_bag()
{
	this->destroy_tree(this->tree);
}

bool searchable_tree_bag::searchAux(node *root, int value) const
{
	if (!root)
		return false;
	if (root->value == value)
		return true;
	if (value < root->value)
		return searchAux(root->l, value);
	return searchAux(root->r, value);

}

bool searchable_tree_bag::has(int value) const
{
	return searchAux(this->tree, value);
}
