#pragma once

#include "number_type_base.h"

class number_type_integer : public number_type_base
{
	static const int MY_SIZE = 2;

public:
	number_type_integer();

	virtual std::string get_initializer() const;
	virtual std::string get_initializer(const std::string& number) const;
};