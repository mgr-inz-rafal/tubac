#include "variable_token.h"

variable_token::variable_token(const std::string& label_): token(label_)
{};

bool variable_token::should_spawn() const
{
	return true;
}

