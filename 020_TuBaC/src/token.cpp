#include "token.h"

token::token(const std::string& label_): label(TOKEN_INDICATOR + label_)
{}

const std::string& token::get_label() const
{
	return label;
}

bool token::should_spawn() const
{
	return false;
}

bool token::should_spawn_on_zero_page() const
{
	return false;
}
