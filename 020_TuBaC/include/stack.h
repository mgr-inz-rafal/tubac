#pragma once

#include <string>

class stack
{
	std::string name;
	std::string pointer;
	int item_size;
	int capacity;
public:
	stack(const std::string& _name, int _item_size, int _capacity);
	const std::string& get_name() const;
	const std::string& get_pointer() const;
	int get_capacity() const;
	int get_item_size() const;
};

