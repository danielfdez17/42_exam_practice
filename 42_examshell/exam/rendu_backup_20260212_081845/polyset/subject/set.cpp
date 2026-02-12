#include "set.hpp"

// set::set()
// {

// }
set::~set()
{

}
// set::set(set const&src)
// {
// 	if (this != &src)
// 	{
// 		this->my_bag = src.my_bag;
// 	}
// }
// set set::operator=(set const& src)
// {
// 	if (this != &src)
// 	{
// 		this->my_bag = src.my_bag;
// 	}
// 	return *this;
// }
set::set(searchable_bag &bag)
{
	this->my_bag = &bag;
}
void set::insert(int n)
{
	if (!this->has(n))
		this->my_bag->insert(n);
}
void set::insert(int *data, int n)
{
	for (int i = 0; i < n; i++)
		this->insert(data[i]);
}
bool set::has(int value) const
{
	return this->my_bag->has(value);
}
void set::print() const
{
	my_bag->print();
}
void set::clear()
{
	my_bag.clear();
}
const searchable_bag& set::get_bag()
{
	return *my_bag;
}
