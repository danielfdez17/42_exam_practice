#include "searchable_array_bag.hpp"

searchable_array_bag::searchable_array_bag() : array_bag()
{
	this->size = 0;
	this->data = nullptr;
}
searchable_array_bag::searchable_array_bag(const searchable_array_bag &src) : array_bag(src)
{
	size = src.size;
	data = new int[size];
	for (int i = 0; i < size; i++) {
		data[i] = src.data[i];
	}
}
searchable_array_bag &searchable_array_bag::operator=(const searchable_array_bag &src)
{
	if (this != &src) {
		if (data != nullptr) {
			delete[] data;
			data = nullptr;
		}
		size = src.size;
		data = new int[size];
		for (int i = 0; i < size; i++) {
			data[i] = src.data[i];
		}
	}
	return *this;
}
searchable_array_bag::~searchable_array_bag()
{
	if (data != nullptr) {
		delete[] data;
		data = nullptr;
	}
}

bool searchable_array_bag::has(int value) const
{
	for (int i = 0; i < this->size; i++)
		if (this->data[i] == value)
			return true;
	return false;
}
