#include "token.h"

token::token(const std::string& label_, bool spawn_as_variable_, bool spawn_on_zero_page_):
	label(label_),
	spawn_as_variable(spawn_as_variable_),
	spawn_on_zero_page(spawn_on_zero_page_)
{}

const std::string& token::get_label() const
{
	return label;
}

bool token::should_spawn() const
{
	return spawn_as_variable;
}

bool token::should_spawn_on_zero_page() const
{
	return spawn_on_zero_page;
}
