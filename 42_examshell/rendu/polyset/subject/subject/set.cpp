#include "set.hpp"

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
