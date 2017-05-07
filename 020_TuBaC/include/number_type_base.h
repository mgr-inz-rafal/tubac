#pragma once

#include <string>

class number_type_base
{
	int size;

public:
	number_type_base(int _size);
	virtual int get_size() const;
	virtual std::string get_initializer() const = 0;
	virtual std::string get_initializer(const std::string& number) const = 0;
};