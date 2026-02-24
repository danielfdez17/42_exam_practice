#include "searchable_array_bag.hpp"
#include "searchable_tree_bag.hpp"
#include "set.hpp"

searchable_array_bag::searchable_array_bag() : array_bag()
{
	this->size = 0;
	this->data = nullptr;
}
searchable_array_bag::searchable_array_bag(const searchable_array_bag &src) : array_bag(src)
{
	this->size = src.size;
	this->data = new int[this->size];
	for (int i = 0; i < this->size; i++) {
		this->data[i] = src.data[i];
	}
}
searchable_array_bag &searchable_array_bag::operator=(const searchable_array_bag &src)
{
	if (this != &src) {
		if (this->data != nullptr) {
			delete[] this->data;
			this->data = nullptr;
		}
		this->size = src.size;
		this->data = new int[size];
		for (int i = 0; i < this->size; i++) {
			this->data[i] = src.data[i];
		}
	}
	return *this;
}
searchable_array_bag::~searchable_array_bag()
{
	if (this->data != nullptr) {
		delete[] this->data;
		this->data = nullptr;
	}
}

bool searchable_array_bag::has(int value) const
{
	for (int i = 0; i < this->size; i++)
		if (this->data[i] == value)
			return true;
	return false;
}

searchable_tree_bag::searchable_tree_bag() : tree_bag()
{
	this->tree = nullptr;
}
searchable_tree_bag::searchable_tree_bag(const searchable_tree_bag &src) : tree_bag(src)
{
	this->tree = copy_node(src.tree);
}
searchable_tree_bag &searchable_tree_bag::operator=(const searchable_tree_bag &src)
{
	if (this != &src) {
		destroy_tree(this->tree);
		this->tree = copy_node(src.tree);
	}
	return *this;
}
searchable_tree_bag::~searchable_tree_bag()
{
	destroy_tree(this->tree);
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
	return this->searchAux(this->tree, value);
}


// set::set()
// {

// }
set::set(searchable_bag &bag) : mybag(bag)
{

}
// set::set(const set &)
// {

// }
// set &set::operator=(const set &other)
// {

// }
set::~set()
{

}
void set::insert (int value)
{
	if (!this->has(value))
		this->mybag.insert(value);
}
void set::insert (int *data, int size)
{
	for (int i = 0; i < size; i++)
		this->insert(data[i]);
}
void set::print() const
{
	this->mybag.print();
}
void set::clear()
{
	this->mybag.clear();
}
bool set::has(int value) const
{
	return this->mybag.has(value);
}
searchable_bag &set::get_bag() const
{
	return this->mybag;
}
