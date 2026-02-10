#include "searchable_array_bag.hpp"

searchable_array_bag::searchable_array_bag() : array_bag()
{
	this->size = 0;
	this->data = nullptr;
}
searchable_array_bag::searchable_array_bag(const searchable_array_bag &copy) : array_bag(copy)
{
	this->size = copy.size;
	this->data = new int[this->size];
	for (int i = 0; i < this->size; i++) {
		this->data[i] = copy.data[i];
	}
}
searchable_array_bag::~searchable_array_bag()
{
	if (this->data != nullptr) {
		delete[] this->data;
		this->data = nullptr;
		this->size = 0;
	}
}
searchable_array_bag &searchable_array_bag::operator=(const searchable_array_bag &copy)
{
	if (this != &copy) {
		if (data != nullptr) {
			delete[] data;
			data = nullptr;
		}
		size = copy.size;
		data = new int[size];
		for (int i = 0; i < size; i++) {
			data[i] = copy.data[i];
		}
	}
	return *this;
}

bool	searchable_array_bag::has(int value) const
{
	for (int i = 0; i < this->size; i++)
	{
		if (this->data[i] == value)
			return true;
	}
	return false;
}
