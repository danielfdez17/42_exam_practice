#include "searchable_array_bag.hpp"

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
